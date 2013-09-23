// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef FEATURESMAP_H
#define FEATURESMAP_H

#include <fstream>
#include <boost/filesystem.hpp>

#include "findmf/datastruct/featurereadadapterfm.h"

namespace ralab{
  //returns with idx

  /** List of features */
  struct FeaturesMap
  {

    typedef std::vector<Feature2D > Features;
  private:
    MapLCMSDescriptionPtr mapdescription_; // map description

    Features features_;
    std::pair<double, double> swathw_; //the rttimes

  public :

    FeaturesMap(){}
    FeaturesMap(MapLCMSDescriptionPtr mapdescription ):mapdescription_(mapdescription), features_(){}
    FeaturesMap(std::size_t & s):features_(s){}


    void setMapDescription(MapLCMSDescriptionPtr map){
      mapdescription_ = map;
    }

    MapLCMSDescriptionPtr getMapDescription(){
      return mapdescription_;
    }

    void resize(std::size_t size ){
      features_.resize(size);
    }

    Feature2D & at(std::size_t i){
      return features_[i];
    }

    std::size_t size(){
      return features_.size();
    }


    Features & features(){
      return features_;
    }


  };
}


#endif // FEATURESMAP_H
