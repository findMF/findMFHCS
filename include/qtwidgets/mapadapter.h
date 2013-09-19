// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef MAPADAPTER_H
#define MAPADAPTER_H

#include <vigra/multi_array.hxx>
#include <vigra/inspectimage.hxx>


#include "findmf/datastruct/lcmsimage.h"

namespace ralab{


  /**/
  struct MapVis{
    virtual ~MapVis(){}
    virtual std::size_t getMZsize() = 0;
    virtual std::size_t getRTsize() = 0;
    virtual float get(std::size_t px, std::size_t py) = 0;
    virtual float getMaxelem() = 0;
  };

  /**
    used to visualize segementation.
  */
  template<typename T>
  struct AbstractMultiArrayVis : MapVis{
    typedef vigra::MultiArray<2,T> DATA;
    typedef typename DATA::difference_type difftype;
    DATA data_;
    float max_;


    AbstractMultiArrayVis(DATA & data){
      data_ = data;
      //inspect image
      vigra::FindMinMax<typename DATA::value_type> minmax;   // functor to find range
      inspectImage(srcImageRange(data), minmax); // find original range
      max_ = minmax.max;
    }

    virtual std::size_t getMZsize()override{
      return data_.size(0);
    }

    virtual std::size_t getRTsize()override{
      return data_.size(1);
    }

    //template mehtods
    virtual float get(std::size_t px, std::size_t py) override = 0;

    //template methods
    virtual float getMaxelem() override = 0;
  };

  /**
    used to visualize segementation.
  */
  template<typename T>
  struct MultiArrayVisSegments : public AbstractMultiArrayVis<T>{

    typedef  AbstractMultiArrayVis<T> Base;

    MultiArrayVisSegments( typename Base::DATA & data): Base(data){ }

    virtual float get(std::size_t px, std::size_t py)override{
      int x = Base::data_[typename Base::difftype(px,py)];
      if(x % 2 == 0)
        return x;
      else
        return Base::max_ - x;
    }

    virtual float getMaxelem()override{
      return Base::max_;
    }
  };


  // square root visualization of image.
  template<typename T>
  struct MultiArrayVisSquare : AbstractMultiArrayVis<T>{
    typedef  AbstractMultiArrayVis<T> Base;

    MultiArrayVisSquare(typename Base::DATA & data):Base(data){
    }

    virtual float get(std::size_t px, std::size_t py)override{
      float x = Base::data_[typename Base::difftype(px,py)];
      return x*x;
    }

    virtual float getMaxelem()override{
      return Base::max_*Base::max_;
    }
  };

  /**

  */
  template<typename T>
  struct MultiArrayVisLog : AbstractMultiArrayVis<T>{
    typedef  AbstractMultiArrayVis<T> Base;
    MultiArrayVisLog(typename Base::DATA & data):Base(data){}

    virtual float get(std::size_t px, std::size_t py) override {
      return log( Base::data_[typename Base::difftype(px,py)]+1.);
    }

    virtual float getMaxelem() override {
      return log(Base::max_+1.);
    }
  };

}
#endif // MAPADAPTER_H
