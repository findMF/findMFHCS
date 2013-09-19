// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


//author Witold Wolski

#ifndef IFEATURERANGEACCESS_H
#define IFEATURERANGEACCESS_H

#include <stdint.h>

namespace ralab{

  struct IFeatureRangeAccess{
    ~IFeatureRangeAccess(){}
    virtual uint32_t getID() = 0;
    virtual float getMinMZsw()  = 0;
    virtual float getMaxMZsw() = 0;
    virtual float getMinRT() = 0;
    virtual float getMaxRT() = 0;
    virtual float getMinMZ() = 0;
    virtual float getMaxMZ() = 0;
  };






}//end namespace

#endif // IFEATURERANGEACCESS_H
