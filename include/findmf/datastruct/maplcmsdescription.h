// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


//author Witold Wolski

#ifndef MAPLCMSDESCRIPTION_H
#define MAPLCMSDESCRIPTION_H

#include <utility>
#include <boost/shared_ptr.hpp>
#include "base/base/equispaceinterpolation.h"

namespace ralab{
  ///
  /// Description of an lcms map...
  ///
  struct MapLCMSDescription
  {
    uint32_t mslevel_;
    std::pair<double, double> extractionWindowMZ_; //swath window
    std::vector<std::size_t> indices_; // indices of the spectra in the raw file.
    std::pair<double, double> mzRange_;
    std::pair<double, double> rtRange_;

    typedef std::vector<double> axesT;
    axesT retentiontime_;//retention times in s (index to rt)
    axesT mass_; //mass vector (is used convert index to mass)

    MapLCMSDescription():mslevel_(0),extractionWindowMZ_(),indices_(),
      mzRange_(),rtRange_(),retentiontime_(),mass_(){
      mzRange_ = std::make_pair(std::numeric_limits<double>::max(),std::numeric_limits<double>::min());
      rtRange_ = std::make_pair(std::numeric_limits<double>::max(),std::numeric_limits<double>::min());
    }

    // use for transforming index to RT
    double getRTforIndex(double index){
      double res = 0;
      ralab::base::base::interpolate_linear( retentiontime_.begin() , retentiontime_.end() , &index,  &index+1 , &res);
      return res;
    }

    //use for transforming index to mz
    double getMZforIndex(double index){
      double res = 0;
      ralab::base::base::interpolate_linear( mass_.begin() , mass_.end() , &index, &index+1, &res );
      return res;
    }

    void setRT(const std::vector<double> & retentiontime){
      retentiontime_ = retentiontime;
    }

    std::vector<double> & getRT(){
      return retentiontime_;
    }

    //convinience setters

    uint32_t getMSLevel(){
      return mslevel_;
    }

    //extraction window start
    double getMinMZsw(){
      return extractionWindowMZ_.first;
    }

    //extraction window end
    double getMaxMZsw(){
      return extractionWindowMZ_.second;
    }

    // min mz
    double getMinMZ(){
      return mzRange_.first;
    }
    // max mz
    double getMaxMZ(){
      return mzRange_.second;
    }
    // min rt
    double getMinRT(){
      return rtRange_.first;
    }

    // max rt
    double getMaxRT(){
      return rtRange_.second;
    }


    //get the transformation vectors
    char * getRTAxis(std::size_t & size) override {
      //char * res = boost::lexical_cast<char *>(feature_.getProjectionRT());
      char * res = reinterpret_cast<char *>( &(retentiontime_[0]) );
      size = retentiontime_.size() * (sizeof(axesT::value_type)/sizeof(char));
      return res;
    }

    //get the transformation vectors
    char * getMZAxis(std::size_t & size) override {
      //char * res = boost::lexical_cast<char *>(feature_.getProjectionMZ());
      char * res = reinterpret_cast<char *>( &(mass_[0]) );
      size = mass_.size() * (sizeof(axesT::value_type)/sizeof(char));
      return res;
    }
  };//end MapLCMSDescription
  typedef boost::shared_ptr<MapLCMSDescription> MapLCMSDescriptionPtr;
}
#endif // MAPLCMSDESCRIPTION_H
