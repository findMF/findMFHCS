// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef FEATURE_H
#define FEATURE_H

#include <vector>

#include <boost/bind.hpp>
#include <stdint.h>

namespace ralab{


  struct IFeatureAccess{
    ~IFeatureAccess(){}
    virtual uint32_t getID()  = 0;

    virtual uint32_t getSwathId() = 0;

    //intensities
    virtual float getVolume() = 0;
    virtual float getMaximum() = 0;
    virtual uint32_t getCount() = 0;

    //location
    virtual float getApexMZ() = 0;
    virtual float getApexRT() = 0;
    virtual float getCenterOfMassMZ() = 0;
    virtual float getCenterOfMassRT() = 0;
    virtual float getMaxLocationMZ() = 0;
    virtual float getMaxLocationRT() = 0;

    //distribution infromation
    virtual float getSDRT() = 0;
    virtual float getSDMZ() = 0;
    virtual float getKurtosisRT() = 0;
    virtual float getKurtosisMZ() = 0;
    virtual float getSkewnessRT() = 0;
    virtual float getSkewnessMZ() = 0;

    //bounding box
    virtual float getMinRTIdx() = 0;
    virtual float getMinMZIdx() = 0;
    virtual float getMZExtend() = 0;
    virtual float getRTExtend() = 0;

    //projections
    virtual char * getRTProjectionData(std::size_t & size) = 0;
    virtual char * getMZProjectionData(std::size_t & size) = 0;

    //
    virtual std::vector<float> & getRTProjection() = 0;
    virtual std::vector<float> & getMZProjection() = 0;
  };



}



#endif // FEATURE_H
