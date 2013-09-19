// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


//author Witold Wolski



#ifndef FEATUREREADADAPTER_H
#define FEATUREREADADAPTER_H

#include "findmf/interfaces/ifeatureallaccess.h"
#include "findmf/datastruct/feature2d.h"

namespace ralab{

  //
  //returns with idx
  //

  struct FeatureReadAdapter : IFeatureAllAccess {
    Feature2D & feature_;
    ralab::projectionstats & mzstats_;
    ralab::projectionstats & rtstats_;

    FeatureReadAdapter(Feature2D & feature)
      :feature_(feature),
        mzstats_(feature.getMZStats()),
        rtstats_(feature.getRTStats())
    {}

    uint32_t getID() override {
      return static_cast<uint32_t>(feature_.getID());
    }

    uint32_t getSwathId(){
      return feature_.getSwathId();
    }

    //implementing the IFeatureRangeAccess interface
    virtual float getMinRT(){
      float rtidx = this->getMinRTIdx();
      return rtidx;
    }

    virtual float getMaxRT(){
      return (this->getMinRTIdx() + this->getRTExtend());
    }

    virtual float getMinMZ(){
      return this->getMinMZIdx();
    }

    virtual float getMaxMZ(){
      return (this->getMinMZIdx() + this->getMZExtend());
    }

    virtual float getMinMZsw(){
      return (feature_.getMinMZsw());
    }

    virtual float getMaxMZsw(){
      return(feature_.getMaxMZsw());
    }
    ///implmenting the IFeatureAccess Interface


    //intensities
    float getVolume() override {
      return feature_.getVolume();
    }
    float getMaximum() override {
      return feature_.getMaximum();
    }

    uint32_t getCount() override {
      return feature_.getCount();
    }

    //location
    float getApexMZ() override {
      return mzstats_.getApex();
    }
    float getApexRT() override {
      return rtstats_.getApex();
    }

    float getCenterOfMassMZ() override {
      return feature_.getCenterOfMassMZ();
    }
    float getCenterOfMassRT() override {
      return feature_.getCenterOfMassRT();
    }
    float getMaxLocationMZ() override {
      return feature_.getMaxLocationMZ();
    }
    float getMaxLocationRT() override {
      return feature_.getMaxLocationRT();
    }

    //distribution infromation
    float getSDRT() override {
      return rtstats_.getSD();
    }
    float getSDMZ() override {
      return mzstats_.getSD();
    }

    float getKurtosisRT() override {
      return rtstats_.getKurtosis();
    }

    float getKurtosisMZ() override {
      return mzstats_.getKurtosis();
    }

    float getSkewnessRT() override {
      return rtstats_.getSkewness();
    }

    float getSkewnessMZ() override {
      return mzstats_.getSkewness();
    }

    //bounding box
    float getMinRTIdx() override {
      return feature_.getMinRTIdx();
    }

    float getMinMZIdx() override {
      return feature_.getMinMZIdx();
    }

    float getMZExtend() override {
      return feature_.getMZExtend();
    }

    float getRTExtend() override {
      return feature_.getRTExtend();
    }

    char * getRTProjectionData(std::size_t & size) override {
      //char * res = boost::lexical_cast<char *>(feature_.getProjectionRT());
      char * res = reinterpret_cast<char *>( &(feature_.getProjectionRT())[0] );
      size = feature_.getProjectionRT().size() * (sizeof(Feature2D::value_type)/sizeof(char));
      return res;
    }

    char * getMZProjectionData(std::size_t & size) override {
      //char * res = boost::lexical_cast<char *>(feature_.getProjectionMZ());
      char * res = reinterpret_cast<char *>( &(feature_.getProjectionMZ())[0] );
      size = feature_.getProjectionMZ().size() * (sizeof(Feature2D::value_type)/sizeof(char));
      return res;
    }

    std::vector<float> & getMZProjection() override {
      return feature_.getProjectionMZ();
    }

    std::vector<float> & getRTProjection() override {
      return feature_.getProjectionRT();
    }
  };


}//end ralab
#endif // FEATUREREADADAPTER_H
