// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef MAPADAPTER_H
#define MAPADAPTER_H

#include <vigra/multi_array.hxx>
#include <vigra/inspectimage.hxx>
#include <boost/math/special_functions/asinh.hpp>

#include "findmf/datastruct/lcmsimage.h"
#include "qtwidgets/MapVis.h"


namespace ralab{

  ///  used to visualize segementation.
  template<typename T>
  struct AbstractMultiArrayVis : MapVis{
    typedef vigra::MultiArray<2,T> DATA;
    typedef typename DATA::difference_type difftype;
    DATA data_;
    float max_;
    float min_;


    AbstractMultiArrayVis(DATA & data){
      data_ = data;
      //inspect image
      vigra::FindMinMax<typename DATA::value_type> minmax;   // functor to find range
      inspectImage(srcImageRange(data), minmax); // find original range
      max_ = minmax.max;
      min_ = minmax.min;
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
    virtual float getMinelem() override = 0;
  };

  /// used to visualize segementation.
  template<typename T>
  struct MultiArrayVisSegments : public AbstractMultiArrayVis<T>{

    typedef  AbstractMultiArrayVis<T> Base;

    MultiArrayVisSegments( typename Base::DATA & data): Base(data){ }

    virtual float get(std::size_t px, std::size_t py)override{
      int x = Base::data_[typename Base::difftype(px,py)];

      double max = 8.;
      int maxi = 8;
      if(x == 0)
        return Base::max_;
      else if(x % maxi == 0)
        return Base::max_ * (0.5/max);
      else if((x+1)%maxi ==0)
        return Base::max_ * (2./max);
      else if((x+2)%maxi ==0)
        return Base::max_ * (3./max);
      else if((x+3)%maxi ==0)
        return Base::max_ * (4./max);
      else if((x+4)%maxi ==0)
        return Base::max_ * (5./max);
      else if((x+5)%maxi ==0)
        return Base::max_ * (6./max);
      else if((x+6)%maxi ==0)
        return Base::max_ * (7./max);
      else
        return Base::max_ * (7.5/max);
    }

    virtual float getMaxelem()override{
      return Base::max_;
    }

    virtual float getMinelem() override{
      return Base::min_;
    }
  };



  /// arcus sinus hyperbolicus visualization of image.
  template<typename T>
  struct MultiArrayVisAsinh : AbstractMultiArrayVis<T>{
    typedef  AbstractMultiArrayVis<T> Base;

    MultiArrayVisAsinh(typename Base::DATA & data):Base(data){
    }

    virtual float get(std::size_t px, std::size_t py)override{
      float x = Base::data_[typename Base::difftype(px,py)];
      return boost::math::asinh(x);
    }

    virtual float getMaxelem()override{
      return boost::math::asinh(Base::max_);
    }
    virtual float getMinelem()override{
      return boost::math::asinh(Base::min_);
    }
  };

  /// square root visualization of image.
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
    virtual float getMinelem()override{
      return Base::min_*Base::min_;
    }
  };

  ///
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

    virtual float getMinelem() override {
      return log(Base::min_+1.);
    }
  };

}
#endif // MAPADAPTER_H
