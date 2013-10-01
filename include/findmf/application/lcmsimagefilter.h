// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef LCMSIMAGEFILTER_H
#define LCMSIMAGEFILTER_H

#include "findmf/datastruct/lcmsimage.h"
#include "base/filter/scanfilter/getfitlertophat.h"
#include <vigra/stdimagefunctions.hxx>
#include <vigra/convolution.hxx>

namespace ralab{
  class LCMSImageFilter{
  private:
    std::vector<float> signal_;

    static void filter( ralab::LCMSImage::Map & mp_,float scalemz = 1. , float scalert = 1.){
      //using namespace vigra::functor;
      vigra::gaussianSmoothing(vigra::srcImageRange(mp_), vigra::destImage(mp_), scalemz , scalert);
    }

    /** square map intensities*/
    static void sq(ralab::LCMSImage::Map & mp_){
      vigra::transformImage(srcImageRange(mp_),
                            destImage(mp_),vigra::functor::sq(vigra::functor::Arg1())
                            );
    }

    static void sqrt(LCMSImage::Map & mp_){
      vigra::transformImage(srcImageRange(mp_),
                            destImage(mp_), vigra::functor::sqrt(vigra::functor::Arg1())
                            );
    }

    void filterRT(LCMSImage::Map & mp_, ralab::base::filter::scanfilter::IScanFilterFloatPtr  f ) override {
      size_t xx = mp_.size(0);
      for( std::size_t i = 0 ; i < xx; ++i )
        {
          signal_.assign( mp_.bindInner(i).begin(), mp_.bindInner(i).end() );
          (*f)( signal_ );
          //std::replace_if( signal_.begin() , signal_.end() , std::bind2nd(std::less<float>(),0.),0.);
          std::copy(signal_.begin(),signal_.end(),mp_.bindInner(i).begin() );
        }
    }

    //apply filter to all spectra in MZ dimension
    void filterMZ(LCMSImage::Map & mp_, ralab::base::filter::scanfilter::IScanFilterFloatPtr  f ) override{
      for(std::ptrdiff_t i = 0 ; i < mp_.size(1); ++i)
        {
          signal_.assign( mp_.bindOuter(i).begin(), mp_.bindOuter(i).end() );
          (*f)( signal_ );
          //std::replace_if( signal_.begin() , signal_.end() , std::bind2nd(std::less<float>(),0.),0.);
          std::copy(signal_.begin(),signal_.end(),mp_.bindOuter(i).begin());
        }
    }

  public:
    void filterMap( ralab::LCMSImage & mp_, //!<
                    uint32_t mzpixelwidth, //!< in pixel
                    uint32_t rtpixelwidth, //!< in pixel
                    double mzscale=1., //!< scale
                    double rtscale=1.,
                    double factor = 2.5 // size of structuring element resolution * factor
        )
    {
      LOG(INFO) << "mzscale: " << mzscale << " rtscale " << rtscale << " mzw " << mzpixelwidth << "rt" << rtpixelwidth <<  " factor : " << factor << std::endl;
      sqrt(mp_.getMap()); // put it on nicer scale
      filter(mp_.getMap(),mzscale,rtscale);
      mp_.updateMax();
      if(rtpixelwidth > 0){
          ralab::base::filter::scanfilter::IScanFilterFloatPtr sfpfRT =
              ralab::base::filter::scanfilter::getFilterTOPHAT(
                rtpixelwidth
                ,factor
                );
          filterRT(mp_.getMap(),sfpfRT);
        }
      if(mzpixelwidth > 0){
          ralab::base::filter::scanfilter::IScanFilterFloatPtr sfpfMZ =
             ralab::base::filter::scanfilter::getFilterTOPHAT(
                mzpixelwidth
                ,factor
                );
          filterMZ(mp_.getMap(),sfpfMZ);
        }
      //in addition filter after background removal
      filter(mp_.getMap(),mzscale/2., rtscale/2.);
      sq(mp_.getMap());
      mp_.updateMax();
    }


  };
}//end namespace
#endif // LCMSIMAGEFILTER_H
