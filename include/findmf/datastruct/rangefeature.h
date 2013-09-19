// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef RANGEACCESS_H
#define RANGEACCESS_H

#include "findmf/interfaces/ifeaturerangeaccess.h"

namespace ralab{
  struct RangeFeature : IFeatureRangeAccess{
  private:
    uint32_t id_;
    float minRT_;
    float maxRT_;
    float minMZ_;
    float maxMZ_;
    float minMZsw_;
    float maxMZsw_;

  public:

    void setID(uint32_t id){ id_ = id;}
    uint32_t getID() override { return id_;}

    void setMinRT(float minRT) { minRT_ = minRT;}
    float getMinRT() override { return minRT_;}

    void setMaxRT(float maxRT) { maxRT_ = maxRT;}
    float getMaxRT() override { return maxRT_;}

    void setMinMZ(float minMZ) { minMZ_ = minMZ;}
    float getMinMZ() override { return minMZ_;}

    void setMaxMZ(float maxMZ) { maxMZ_ = maxMZ;}
    float getMaxMZ() override { return maxMZ_;}

    virtual float getMinMZsw(){
      return minMZsw_;
    }
    virtual float getMaxMZsw(){
      return maxMZsw_;
    }

  };

}//end namespace
#endif // RANGEACCESS_H
