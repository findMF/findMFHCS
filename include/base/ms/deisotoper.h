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

      template < class Iterator, class T >
      Iterator findNearestNeighbor(Iterator first, Iterator last, const T & val) {
        Iterator it = std::upper_bound(first, last, val);

        if (it != first) {
          Iterator it_pre = it - 1;
          if (fabs(val - *it_pre) < fabs(val - *it) || it == last) {
            return (it_pre);
          }
        }
        return (it);
      }


      class Deisotoper{
        std::vector < int > Z_; // charge states
        std::vector< std::vector < double > > isotopPatternMap;
        double ppmError_;
        double hydrogenMass_;
        ralab::base::chemistry::IIsotopeEnvelope * ie_;

      public:

        typedef std::vector< std::vector< std::vector < int > > > ListOfVectors;
        typedef std::vector< std::vector < double > > ListOfDoubleVectors;

        struct IsotopResult {
          ListOfVectors isotopChains;
          ListOfDoubleVectors isotopDotProducts;

          std::vector < std::vector < int > >isotopPatternIdx;
          std::vector < std::vector < double > >isotopPeakScores;
          std::vector < double >isotopScores;

          std::vector < std::vector < int > >isotopGroups;

          const std::vector < int > & getIsotopPatten(int i) {
            if (isotopPatternIdx[i].size() > 0)
              return isotopPatternIdx[i];
          }};

        IsotopResult result_;

        const std::vector < std::vector < int > >  & getIsotopGroups() const{
          return result_.isotopGroups;
        }

        const ListOfVectors & getIsotopChainResults() const{
          return result_.isotopChains;
        }

        const ListOfDoubleVectors & getIsotopDotProductsResults() const{
          return result_.isotopDotProducts;
        }


        // constructor
        Deisotoper(double ppm = 50 ):ppmError_(ppm),hydrogenMass_(1.008){}

        template<class TintI>
        Deisotoper(TintI beginz , TintI endz, double ppm = 50):
          Z_( beginz , endz ),
          ppmError_(ppm),hydrogenMass_(1.008)
        {}

        /// set isotope envelope access (inject dependency)
        void setIsotopPatternMap(ralab::base::chemistry::IIsotopeEnvelope * isotopeEnvlope){
          ie_ = isotopeEnvlope;
        }


        template < typename TmassI, typename TinteI >
        void computeIsotopChains(TmassI beginMass, TmassI endMass)//, TinteI beginIntensity)
        {

          std::vector < int > isoptopGroupsIdx;

          std::vector <std::vector < int > >isoptopGroupsIdxChargeBegin(Z_.size());
          std::vector <std::vector < int > >isoptopGroupsIdxChargeEnd(Z_.size());

          std::vector<std::vector< int > > results ;
          for (unsigned int zi = 0; zi < Z_.size(); zi++) {


            computeDeisotopCandidates(beginMass, endMass, Z_[zi], results);

            result_.isotopChains.push_back(results);

            // preprocessing for ISOTOP GROUPING
            // step 0: determine all isotop groups for all charge states
            for (std::vector<std::vector< int > >::iterator it = results.begin(); it !=  results.end(); ++it){
              isoptopGroupsIdx.insert(isoptopGroupsIdx.end(), (*it).begin(), ++(*it).begin());

              std::vector< int >::iterator end__ = (*it).end();
              --end__;

              isoptopGroupsIdxChargeBegin[zi].push_back(*(*it).begin());
              isoptopGroupsIdxChargeEnd[zi].push_back(*end__);

              (*it).clear();
            }
            results.clear();
          }

          // ISOTOP GROUPING
          std::sort (isoptopGroupsIdx.begin(), isoptopGroupsIdx.end());
          std::unique (isoptopGroupsIdx.begin(), isoptopGroupsIdx.end());

          std::vector<int>::iterator it_NN_begin;
          std::vector<int>::iterator it_NN_end;

          // for all isotop groups
          for(std::vector< int >::iterator it = isoptopGroupsIdx.begin(); it !=isoptopGroupsIdx.end(); ++it){

            // default -1 mean NA means no isotop for this charge state
            std::vector< int > group(Z_.size(), -1);

            // for all charge states
            for (unsigned int zi = 0; zi < Z_.size(); zi++) {

              // if we have a isotop group for charge state zi
              if (std::distance(isoptopGroupsIdxChargeBegin[zi].begin(), isoptopGroupsIdxChargeBegin[zi].end()) > 0){

                // compute lower and upper bound for a isotop group intersection
                it_NN_begin = std::lower_bound(isoptopGroupsIdxChargeBegin[zi].begin(), isoptopGroupsIdxChargeBegin[zi].end(), *it);
                it_NN_end = std::upper_bound(isoptopGroupsIdxChargeEnd[zi].begin(), isoptopGroupsIdxChargeEnd[zi].end(), *it);

                // the end of the list is not the last entry!
                if (it_NN_begin == isoptopGroupsIdxChargeBegin[zi].end())
                  --it_NN_begin;
                if (it_NN_end == isoptopGroupsIdxChargeEnd[zi].end())
                  --it_NN_end;


                // determine if thhere is a isotop group intersection;
                if ( (*it_NN_begin) <=  (*it) && (*it) <=  (*it_NN_end) )
                  group[zi] = std::distance(isoptopGroupsIdxChargeBegin[zi].begin(),it_NN_begin);

              }
            }
            // save the result
            result_.isotopGroups.push_back(group);
          }
        }//end computeIsotopChains

        template < typename TmassI, typename TinteI >
        void assignIsotopIntensities(TmassI beginMass, TmassI endMass, TinteI beginIntensity){

          //TinteI endIntensity = beginIntensity + std::distance(beginMass, endMass);

          for (std::vector< std::vector< std::vector< int > > >::iterator it_z = result_.isotopChains.begin(); it_z !=  result_.isotopChains.end(); ++it_z){

            std::vector < double > Lscore;

            for (std::vector< std::vector< int > >::iterator it_c = (*it_z).begin(); it_c !=  (*it_z).end(); ++it_c){

              double queryMass = *(beginMass + *(*it_c).begin());

              std::vector<double> LIsotopeEnvelope = ie_->isotopeEnvelope(queryMass);

              int count = 0;

              double dIntensityDotProduct = 0.0;

              // compute L2 norm for selected isotop chain
              double dIntensityL2 = 0.0;
              for (std::vector< int >::iterator it_idx = (*it_c).begin(); it_idx !=  (*it_c).end(); ++it_idx)
                dIntensityL2 += ( (*(beginIntensity + (*it_idx))) ) * ( (*(beginIntensity + (*it_idx))) );

              dIntensityL2 = sqrt (dIntensityL2);


              // compute dot product between L2 normalized isotop chain intensities and predicted isotop pattern
              for (std::vector< int >::iterator it_idx = (*it_c).begin(); it_idx !=  (*it_c).end(); ++it_idx){

                // TODO here we have to find the mono iso peak!!!
                // than we can operate with a modulo operator
                dIntensityDotProduct += ((*(beginIntensity + (*it_idx)))  / dIntensityL2) * LIsotopeEnvelope[count];

                count++;
              }

              Lscore.push_back(dIntensityDotProduct);
            } // for all chains
            result_.isotopDotProducts.push_back(Lscore);
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
                          Iterator lastMz)
        {
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
