// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef PROJECTIONSTATS_H
#define PROJECTIONSTATS_H

#include <algorithm>
#include <numeric>
#include "base/stats/momentsW.h"
#include "base/ms/simplepicker.h"


namespace ralab
{
  namespace findmf
  {
    struct projstats{
      float projectionStart_; //TODO can likely be removed
      float average_;
      float variance_;
      float kurtosis_;
      float skewness_;
      float peaklockation_;

      //ctor
      projstats( int projectionStart ):
        projectionStart_(projectionStart), average_(0),variance_(0),
        kurtosis_(0),skewness_(0),peaklockation_()
      {}

      //ctor
      projstats():
        projectionStart_(0),average_(0),variance_(0),kurtosis_(0),skewness_(0),peaklockation_()
      {}

      float getApex() const{
        return peaklockation_;
      }

      float getSD() const {
        return sqrt(variance_);
      }

      float getKurtosis() const{
        return kurtosis_;
      }

      float getSkewness() const{
        return skewness_;
      }


    public:
      float getCenterOfMass() const{
        return projectionStart_ + average_ - 1;
      }
    };


    /** computes projection statistics */
    namespace utilities{
      // TODO provide apex peakfinder.
      struct Pick{
        ralab::base::ms::SimplePicker<float> simplePicker_;
        std::vector<double> zerocross_;
        bool problem_; // indicates if there was some problem i.e. no peak could be detected, or
        //more than one apex found

        Pick():zerocross_(5,0.),problem_(false){}
        inline double pick(const std::vector<float> & projection_,
                          projstats & ps
                          )
        {
          if( projection_.size() >= 5 )
            {
              std::size_t t = simplePicker_(
                    projection_.begin(),
                    projection_.end(),
                    zerocross_.begin(),
                    zerocross_.size()
                    );
              if(t!=1){
                  problem_ = true;
                }
              if(t > 0){
                   double x = ps.projectionStart_ + zerocross_[0];
                  return x;
                }
              else{
                  problem_ = true;
                  return ps.getCenterOfMass();
                }
            }else{
              problem_ = true;
              return ps.getCenterOfMass();
            }
        }
      };

      inline void computeStats(const std::vector<float> & projection_ ,
                               int projectionStart,
                               projstats & ps)
      {
        ps.projectionStart_ = projectionStart;
        std::vector<float> positions_;
        ralab::base::base::seq(projection_.size(),positions_);
        ps.average_ = ralab::stats::meanW(positions_.begin(),positions_.end(),projection_.begin());
        ps.variance_ = ralab::stats::varW(positions_.begin(),positions_.end(),projection_.begin(), ps.average_);
        float sd =sqrt( ps.variance_ );
        ps.kurtosis_ = ralab::stats::kurtW(positions_.begin(),positions_.end(),projection_.begin(),ps.average_,sd );
        ps.skewness_ = ralab::stats::skewW(positions_.begin(),positions_.end(),projection_.begin(),ps.average_,sd );
        //  ps.peaklockation_ = pick(projection_,ps);
      }

    }//end utils

  }
}

#endif // PROJECTIONSTATS_H
