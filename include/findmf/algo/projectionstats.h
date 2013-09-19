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

  /**computes projection statistics
*/
  struct projectionstats{

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
      projectionStart_(projectionStart),
      average_(0),variance_(0),kurtosis_(0),skewness_(0),simplePicker_(),zerocross_(2,0.)
    {
      this->computeStats(projection,projectionStart_);
    }


    //ctor
    projectionstats( ):
      projectionStart_(0),
      average_(0),variance_(0),kurtosis_(0),skewness_(0)
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

      // std::vector<float> mzproj(mzprojection_.begin(),mzprojection_.end());
      // std::transform(mzproj.begin(),mzproj.end(),mzproj.begin(),boost::bind(pow,_1,2.));

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

#endif // PROJECTIONSTATS_H
