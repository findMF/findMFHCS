// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


//author Witold Wolski


#ifndef IFEATUREWRITEADAPTER_H
#define IFEATUREWRITEADAPTER_H

#include "findmf/interfaces/ifeaturewrite.h"

namespace ralab{
  // Trivial feature implementation for testing.
  struct FeatureDTO{

    uint32_t id_;
    uint32_t idswath_;
    float volume_;
    float maximum_;
    uint32_t count_;

    float apexMZ_;
    float apexRT_;
    float comMZ_; // center of mass mz
    float comRT_; // center of mass rt
    float maxlocMZ_;// max location mz
    float maxlocRT_;// max location rt

    float sdRT_;
    float sdMZ_;
    float kurtMZ_;
    float kurtRT_;
    float skewMZ_;
    float skewRT_;

    float minRT_;
    float minMZ_;
    float extRT_;
    float extMZ_;

    std::vector<float> rtproj_;
    std::vector<float> mzproj_;

    FeatureDTO():id_(0),idswath_(0),volume_(0.),maximum_(0.),count_(0),
      apexMZ_(0.), apexRT_(0.),comMZ_(0.),comRT_(0.),maxlocMZ_(0.),maxlocRT_(0.),
      sdRT_(0.),sdMZ_(0.),kurtMZ_(0.),kurtRT_(0.),skewMZ_(0.),skewRT_(0.),
      minRT_(0.),minMZ_(0.),extRT_(0.),extMZ_(0.)
    {}

  };

  //writing into a feature implementation
  struct FeatureWriteAdapter : IFeatureWrite {
    FeatureDTO & feature_;

    FeatureWriteAdapter(FeatureDTO & feature)
      :feature_(feature)
    {}

    void setID(uint32_t id) override {
      feature_.id_ = id;
    }

    void setSwathId(uint32_t id) override{
      feature_.idswath_ = id;
    }
    //intensities
    void setVolume(float vol) override {
      feature_.volume_ = vol;
    }

    void setMaximum(float max) override {
      feature_.maximum_ = max;
    }

    void setCount(uint32_t count) override {
      feature_.count_ = count;
    }

    //location
    void setApexMZ(float mz) override {
      feature_.apexMZ_ = mz;
    }


    void setApexRT(float rt) override {
      feature_.apexRT_ = rt;
    }

    void setCenterOfMassMZ(float commz) override {
      feature_.comMZ_ = commz;
    }

    void setCenterOfMassRT(float comrt) override {
      feature_.comRT_  = comrt;
    }

    void setMaxLocationMZ(float maxmz) override {
      feature_.maxlocMZ_  = maxmz;
    }

    void setMaxLocationRT(float maxrt) override {
      feature_.maxlocRT_ = maxrt;
    }


    virtual void setSDRT(float x){
      feature_.sdRT_ = x;
    }
    virtual void setSDMZ(float x){
      feature_.sdMZ_ = x;
    }
    virtual void setKurtosisRT(float x){
      feature_.kurtRT_ =  x;
    }

    virtual void setKurtosisMZ(float x){
      feature_.kurtMZ_ = x;
    }
    virtual void setSkewnessRT(float x){
      feature_.skewRT_ = x;
    }
    virtual void setSkewnessMZ(float x){
      feature_.skewMZ_ = x;
    }

    void setMinMZIdx(float minidx) override{
      feature_.minMZ_ = minidx;
    }

    void setMZExtend(float ext) override{
      feature_.extMZ_ = ext;
    }


    //bounding box
    void setMinRTIdx(float minidx) override {
      feature_.minRT_  = minidx;
    }

    void setRTExtend(float ext) override{
      feature_.extRT_ = ext;
    }


    //spiegel
    void setRTProjectionData(char * xx , std::size_t size) override {
      float * dd = reinterpret_cast<float *>(xx);
      std::size_t x = size/sizeof(float);
      feature_.rtproj_.assign( dd , dd + x );
    }

    void setMZProjectionData(char * xx, std::size_t size) override {
      float * dd = reinterpret_cast<float *>(xx);
      std::size_t x = size/sizeof(float);
      feature_.mzproj_.assign( dd , dd + x );
    }

    void setMZProjection(std::vector<float> &  ) override {
    }

    void setRTProjection(std::vector<float> & ) override {
    }
  };



}//namespace

#endif // IFEATUREWRITEADAPTER_H
