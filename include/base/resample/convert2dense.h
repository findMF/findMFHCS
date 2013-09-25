#ifndef CONVERT2DENSE_H
#define CONVERT2DENSE_H

#include <vector>
#include <iostream>
#include <boost/assert.hpp>
#include "base/resample/masscomparefunctors.h"
#include "base/resample/breakspec.h"
#include "base/resample/bin1d.h"


namespace ralab
{
  namespace base
  {
    namespace resample
    {
      struct Convert2Dense
      {
        ralab::base::resample::Bin1D bin_;
        std::vector<int32_t> idx_;//small workder vecs
        std::vector<double> weight_;
        double am_; //parameter describing the sampling width
        Convert2Dense(double am = 0.1):bin_(),idx_(),weight_(),am_(){
        }

        /// computes split points of an map.
        std::size_t defBreak(std::pair<double, double> & mzrange, double ppm ){
          ralab::base::resample::PPMCompFunctor<double> ppmf(ppm);
          ralab::base::resample::breaks( mzrange.first - 1. , mzrange.second + 1. , ppmf , bin_.breaks_ );
          bin_.reset();
          return bin_.breaks_.size();
        }

        /// Converts a sparse spec to a dense spec
        template<typename Tmass, typename Tintens, typename Tout >
        void convert2dense(Tmass beginMass, Tmass endMass, Tintens intens,
                           Tout ass
                           )
        {

          for( ; beginMass != (endMass -1) ; ++beginMass, ++intens ){
              double mass1 = *beginMass;
              double mass2 = *(beginMass+1);
              double predmass2 = mass1 + (am_* sqrt(mass1))*1.01;
              if(mass2 > predmass2){
                  mass2 = predmass2;
                }

              double deltamass = mass2-mass1;
              double deltamasshalf;
              if(true){
                  deltamasshalf= deltamass/2.;
                }
              else{
                  deltamasshalf = deltamass;
                }

              bin_(mass1-deltamasshalf,mass2-deltamasshalf,idx_,weight_);

              double intensd = static_cast<double>(*intens);
              double sum = std::accumulate(weight_.begin(),weight_.end(),0.);
              BOOST_ASSERT(fabs(deltamass- sum) < 1e-11);

              double check = 0.;
              for(std::size_t i = 0 ; i < idx_.size();++i){
                  if((idx_[i]>=0) &(idx_[i] < static_cast<int32_t>(bin_.breaks_.size() - 1)))
                    {
                      double bb= intensd * weight_[i]/deltamass;
                      *(ass + idx_[i])  += bb;
                      check += bb;
                    }
                }
              BOOST_ASSERT( fabs(check - intensd) < 1e-3 );
            }
        }//convert2dense

        void getMids(std::vector<double> & mids)
        {
          ralab::base::resample::getMids(bin_.breaks_, mids );
        }

        /// Converts a sparse spec to a dense spec
        template<typename Tmass, typename Tintens >
        void convert2dense(Tmass beginMass, Tmass endMass, Tintens intens,
                           std::vector<typename std::iterator_traits<Tintens>::value_type > & gg
                           ){
          gg.resize(bin_.breaks_.size() - 1);
          convert2dense(beginMass,endMass, intens, gg.begin());
        }
      };


    }
  }
}
#endif // CONVERT2DENSE_H
