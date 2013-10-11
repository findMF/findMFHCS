// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef PROJECTIONSTATS_H
#define PROJECTIONSTATS_H

#include <algorithm>
#include <numeric>
#include "base/stats/stats.h"
#include "base/ms/simplepicker.h"

namespace ralab{
  namespace findmf{


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

      /** location of peak maximum
    */
      float getApex() const{
        return peaklockation_;
      }
      /**
    */
      float getSD() const {
        return sqrt(variance_);
      }

      /**
    */
      float getKurtosis() const{
        return kurtosis_;
      }

      /**
    */
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

      struct Pick{
        ralab::base::ms::SimplePicker<float> simplePicker_;
        std::vector<double> zerocross_;
        Pick():zerocross_(5,0.){}

        //TODO - create class
        inline float pick(const std::vector<float> & projection_,
                          projstats & ps
                          )
        {
          if( projection_.size() >= 5 )
            {
              std::size_t t = simplePicker_(
                    projection_.begin(),
                    projection_.end(),
                    zerocross_.begin()
                    );
              if(t > 0){
                  double x = ps.projectionStart_ + zerocross_[0];
                  return x;
                }
              else{
                  return ps.getCenterOfMass();
                }
            }else{
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
        ps.average_ = ralab::stats::WeightedAverage(positions_,projection_);
        ps.variance_ = ralab::stats::VarWeight(positions_,projection_, ps.average_);
        float sd =sqrt( ps.variance_ );
        ps.kurtosis_ = ralab::stats::WeightedKurtosis(positions_,projection_,ps.average_,sd );
        ps.skewness_ = ralab::stats::WeightedSkewness(positions_,projection_,ps.average_,sd );
      //  ps.peaklockation_ = pick(projection_,ps);
      }

    }//end utils





    struct projectionstats
    {
      float projectionStart_;
      float average_;
      float variance_;
      float kurtosis_;
      float skewness_;
      float peaklockation_;

      ralab::base::ms::SimplePicker<float> simplePicker_;
      std::vector<double> zerocross_;
      //ctor
      projectionstats( const std::vector<float> & projection ,  int projectionStart ):
        projectionStart_(projectionStart), average_(0),variance_(0),kurtosis_(0),skewness_(0),peaklockation_(),simplePicker_(),zerocross_(2,0.)
      {
        this->computeStats(projection,projectionStart_);
      }

      //ctor
      projectionstats( ):
        projectionStart_(0),average_(0),variance_(0),kurtosis_(0),skewness_(0),peaklockation_(),simplePicker_(),zerocross_(2,0.)
      {}

      /** location of peak maximum
      */
      float getApex() const{
        return peaklockation_;
      }
      /**
      */
      float getSD() const {
        return sqrt(variance_);
      }

      /**
      */
      float getKurtosis() const{
        return kurtosis_;
      }

      /**
      */
      float getSkewness() const{
        return skewness_;
      }

      /**
      */
      void computeStats(const std::vector<float> & projection_ , int projectionStart)
      {
        projectionStart_ = projectionStart;
        std::vector<float> positions_;
        ralab::base::base::seq(projection_.size(),positions_);
        average_ = ralab::stats::WeightedAverage(positions_,projection_);
        variance_ = ralab::stats::VarWeight(positions_,projection_, average_);
        float sd =sqrt( variance_ );
        kurtosis_ = ralab::stats::WeightedKurtosis(positions_,projection_,average_,sd );
        skewness_ = ralab::stats::WeightedSkewness(positions_,projection_,average_,sd );
        peaklockation_ = pick(projection_);
      }

      float pick(const std::vector<float> & projection_){
        if( projection_.size() >= 5 )
          {
            std::size_t t = simplePicker_(
                  projection_.begin(),
                  projection_.end(),
                  zerocross_.begin()
                  );
            if(t > 0){
                double x = projectionStart_ + zerocross_[0];
                return x;
              }
            else{
                return getCenterOfMass();
              }
          }else{
            return getCenterOfMass();
          }
      }

      /**
      */
    public:
      float getCenterOfMass() const{
        return projectionStart_ + average_ - 1;
      }


    };
  }
}

#endif // PROJECTIONSTATS_H
