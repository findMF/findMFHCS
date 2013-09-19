// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


//author Witold Wolski

#ifndef IFEATUREWRITE_H
#define IFEATUREWRITE_H

namespace ralab{
  struct IFeatureWrite{
    ~IFeatureWrite(){}
    virtual void setID(uint32_t)  = 0;

    virtual void setSwathId(uint32_t) =0;

    //intensities
    virtual void setVolume(float) = 0;
    virtual void setMaximum(float) = 0;
    virtual void setCount(uint32_t) = 0;

    //location
    virtual void setApexMZ(float) = 0;
    virtual void setApexRT(float) = 0;
    virtual void setCenterOfMassMZ(float) = 0;
    virtual void setCenterOfMassRT(float) = 0;
    virtual void setMaxLocationMZ(float) = 0;
    virtual void setMaxLocationRT(float) = 0;

    //distribution infromation

    virtual void setSDRT(float) = 0;
    virtual void setSDMZ(float) = 0;
    virtual void setKurtosisRT(float) = 0;
    virtual void setKurtosisMZ(float) = 0;
    virtual void setSkewnessRT(float) = 0;
    virtual void setSkewnessMZ(float) = 0;

    //bounding box
    virtual void setMinRTIdx(float) = 0;
    virtual void setMinMZIdx(float) = 0;
    virtual void setMZExtend(float) = 0;
    virtual void setRTExtend(float) = 0;

    //projections
    virtual void setRTProjectionData(char * data , std::size_t size) = 0;
    virtual void setMZProjectionData(char * data , std::size_t size) = 0;

    //
    virtual void setRTProjection(std::vector<float> & x) = 0;
    virtual void setMZProjection(std::vector<float> & x) = 0;
  };
}

#endif // IFEATUREWRITE_H
