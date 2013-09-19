// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


//author Witold Wolski


#ifndef FEATUREREADADAPTERFM_H
#define FEATUREREADADAPTERFM_H

#include "findmf/datastruct/feature2d.h"
#include "findmf/datastruct/maplcmsdescription.h"
#include "findmf/datastruct/featurereadadapter.h"

namespace ralab{

  ///
  /// returns retention time and masses instead of pixel locations.
  ///
  struct FeatureReadAdapterFM : FeatureReadAdapter {
    //Feature2D & feature_;
    MapLCMSDescriptionPtr featureMap_; // required here to allow for mz rt transoformations
    //ralab::projectionstats & mzstats_;
    //ralab::projectionstats & rtstats_;

    FeatureReadAdapterFM(Feature2D & feature, MapLCMSDescriptionPtr fm )
      : FeatureReadAdapter( feature ),
        featureMap_( fm )
    {}


    //implementing the IFeatureRangeAccess interface
    virtual float getMinRT(){
      float rtidx = FeatureReadAdapter::getMinRT();
      return featureMap_->getRTforIndex(rtidx);
    }

    virtual float getMaxRT(){
      float idx = FeatureReadAdapter::getMaxRT();
      return featureMap_->getRTforIndex(idx);
    }

    virtual float getMinMZ(){
      float idx=  FeatureReadAdapter::getMinMZ();
      return featureMap_->getMZforIndex(idx);
    }

    virtual float getMaxMZ(){
      float idx=  FeatureReadAdapter::getMaxMZ();
      return featureMap_->getMZforIndex(idx);
    }

    virtual float getMinMZsw(){
      return (featureMap_->extractionWindowMZ_.first);
    }

    virtual float getMaxMZsw(){
      return(featureMap_->extractionWindowMZ_.second);
    }
    ///implmenting the IFeatureAccess Interface


    float getApexMZ() override {
      float apx =  FeatureReadAdapter::getApexMZ();
      return featureMap_->getMZforIndex(apx);
    }

    float getApexRT() override {
      float apx = FeatureReadAdapter::getApexRT();
      return featureMap_->getRTforIndex(apx);
    }

    float getCenterOfMassMZ() override {
      float com = FeatureReadAdapter::getCenterOfMassMZ();
      return featureMap_->getMZforIndex(com);
    }

    float getCenterOfMassRT() override {
      float com = FeatureReadAdapter::getCenterOfMassRT();
      return featureMap_->getRTforIndex(com);
    }


    //returns maximum location
    float getMaxLocationMZ() override {
      float ml = FeatureReadAdapter::getMaxLocationMZ();
      return featureMap_->getMZforIndex(ml);
    }

    float getMaxLocationRT() override {
      float ml =  FeatureReadAdapter::getMaxLocationRT();
      return featureMap_->getRTforIndex(ml);
    }
  };

}//ralab
#endif // FEATUREREADADAPTERFM_H
