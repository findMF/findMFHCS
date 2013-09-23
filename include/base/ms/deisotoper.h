#ifndef DEISOTOPER_H
#define DEISOTOPER_H

#include <vector>

/*

Authors   : Witold Eryk Wolski <wewolski@gmail.com> and Christian Panse <cp@fgcz.ethz.ch>

This code belongs to two projects:
o http://cran.r-project.org/web/packages/protViz/index.html under GPLv3 
o https://github.com/wolski/rl under three-clause BSD license

Copyright : Functional Genomics Center Zurich | UZH | ETHZ and  ETH Zurich 2013

*/

#include <math.h>
#include <stdint.h>

#include <iostream>
#include <vector>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <stack>

#include <base/chemistry/iisotopeenvelope.h>


namespace ralab{
  namespace base{
    namespace ms{

      class Deisotoper {

        std::vector < int > Z_; // charge states
        std::vector<std::vector < double > > isotopPatternMap;

        double ppmError_;
        double hydrogenMass_;
        ralab::base::chemistry::IIsotopeEnvelope * ie_;

      public:

        typedef std::vector< std::vector< std::vector < int > > > ListOfVectors;
        struct isotopResult {
          ListOfVectors isotopChains;
          std::vector < std::vector < int > >isotopPatternIdx;
          std::vector < std::vector < double > >isotopPeakScores;
          std::vector < double >isotopScores;

          const std::vector < int > & getIsotopPatten(int i) {
            if (isotopPatternIdx[i].size() > 0)
              return isotopPatternIdx[i];
          }};

        isotopResult result_;


        const ListOfVectors & getResult() {
          return result_.isotopChains;
        }


        // constructor
        Deisotoper(double ppm = 10 ):ppmError_(ppm),hydrogenMass_(1.008){
          ie_=NULL;
        }

        template<class TintI>
        Deisotoper(TintI beginz , TintI endz, double ppm = 10):
          Z_( beginz , endz ),
          ppmError_(ppm),hydrogenMass_(1.008)
        {}

        void setIsotopPatternMap(ralab::base::chemistry::IIsotopeEnvelope * isotopeEnvlope){
          ie_ = isotopeEnvlope;
        }

        template < typename TmassI, typename TinteI >
        void
        computeIsotopChains(TmassI beginMass, TmassI endMass, TinteI beginIntensity)
        {

          for (unsigned int zi = 0; zi < Z_.size(); zi++) {
              std::vector<std::vector< int > > results ;
              computeDeisotopCandidates(beginMass, endMass, Z_[zi], results);
              result_.isotopChains.push_back(results);
            }
        }

        template < typename TmassI, typename TinteI >
        void
        assignIsotopIntensities(TmassI beginMass, TmassI endMass, TinteI beginIntensity){

          double sumIntensity = 0.0;// *(std::sum_element(beginIntensity, beginIntensity + std::distance(beginMass, endMass)));

          TinteI endIntensity = beginIntensity + std::distance(beginMass, endMass);
          for(TinteI j=beginIntensity; j != endIntensity; ++j){
              sumIntensity += *j;
            }


          for (std::vector< std::vector< std::vector< int > > >::iterator it_z = result_.isotopChains.begin(); it_z !=  result_.isotopChains.end(); ++it_z){
              for (std::vector< std::vector< int > >::iterator it_c = (*it_z).begin(); it_c !=  (*it_z).end(); ++it_c){
                  double queryMass = *(beginMass + *(*it_c).begin());
                  std::cout << "\nqueryMass = " << queryMass;
                  std::vector<double> LISO;
                  ie_ -> isotopeEnvelope(queryMass,LISO);


                  //                    std::cout <<  "\nIsotopPattern:" << std::endl;
                  //                    for( unsigned int k = 0; k < LISO.size(); k++) {
                  //                        std::cout <<  LISO[k] << "\t";
                  //                    }
                  //                    std::cout << std::endl;

                  //                    sumIntensity = 0.0;
                  //                    for (std::vector< int >::iterator it_idx = (*it_c).begin(); it_idx !=  (*it_c).end(); ++it_idx){
                  //                            sumIntensity += *(beginIntensity + (*it_idx));
                  //                    }

                  double dProd = 0.0;
                  double dDotProd = 0.0;
                  int count=0;

                  for (std::vector< int >::iterator it_idx = (*it_c).begin(); it_idx !=  (*it_c).end(); ++it_idx){
                      dProd = ((*(beginIntensity + (*it_idx))) /  sumIntensity) * LISO[count];
                      dDotProd += dProd;

                      std::cout <<"\n" << *it_idx  << "\t" << *(beginMass + (*it_idx)) << "\t" << (*(beginIntensity + (*it_idx))) << "\t" <<  (*(beginIntensity + (*it_idx))) /  sumIntensity  << "\t" << dProd << "\t\t*" << dDotProd << "*\t" << LISO[count]<< std::endl;

                      count++;
                    }

                } // for all chains
            } // for all charge states z
        }


      private:
        int explore(int v, std::vector < int >&G,
                    std::vector < bool > & VISITED,
                    std::vector< int > & result
                    ){

          std::stack < int > S;
          S.push(v);

          while (!S.empty()) {
              v = S.top();
              S.pop();
              result.push_back(v);

              if (!VISITED[v]) {
                  VISITED[v] = true;
                  if (G[v] > -1)
                    S.push(G[v]);
                }
            }

          return 0;
        }

        template < class Iterator, class T > Iterator findNearestNeighbor(Iterator first,
                                                                          Iterator last,
                                                                          const T & val) {
          Iterator it = std::upper_bound(first, last, val);

          if (it != first) {
              Iterator it_pre = it - 1;
              if (fabs(val - *it_pre) < fabs(val - *it) || it == last) {
                  return (it_pre);
                }
            }

          return (it);
        }

        /*
       graph G(V,E);
       each peak represents one node of a G so V = {1, ..., n}
       the edges are defined by (v, node_array_G[v]) iff node_array_G[v] > -1;

       simply speaking an edge is defined between two peaks if the mZ difference is
       below the given ppm error.

       TODO: for efficiency we can have a list containg only the candidates within the ppm error
       having those list we do not have to iterate over the whole graph again.
     */
        template < class Iterator >
        int generateGraph(std::vector <int >&G,
                          int z,
                          Iterator firstMz,
                          Iterator lastMz) {
          for (Iterator itMz = firstMz; itMz != lastMz; ++itMz)
            {

              double dQmZ = (*itMz + (hydrogenMass_ /z));
              Iterator itNNmZ = findNearestNeighbor(firstMz, lastMz, dQmZ);

              if (fabs(*itNNmZ - dQmZ) < (ppmError_ * *itNNmZ * 1E-6))
                G[std::distance(firstMz, itMz)] =
                    std::distance(firstMz, itNNmZ);
            }
          return 0;
        }

        template < typename TmassI > int computeDeisotopCandidates(TmassI beginMass,
                                                                   TmassI endMass,
                                                                   int z,
                                                                   std::vector< std::vector<int> > & candidates) {
          int n = std::distance(beginMass, endMass);
          std::vector < int >G(n, -1);
          generateGraph(G, z, beginMass, endMass);

          std::vector < bool > VISITED(n, false);

          for (unsigned int i = 0; i < G.size(); i++) {
              if (G[i] > -1) {
                  std::vector<int> result;
                  explore(i, G, VISITED,result);
                  if(result.size() > 1){
                      candidates.push_back(result);
                    }
                }
            }
          return 0;
        }

      };
    }
  }
}
#endif				// DEISOTOPER_H
