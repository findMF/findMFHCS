// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef LCMSIMAGEFILTER_H
#define LCMSIMAGEFILTER_H

#include "findmf/datastruct/lcmsimage.h"
#include "findmf/algo/vigra/differenceOfGaussians.h"
#include "base/filter/scanfilter/getfitlertophat.h"

#include <vigra/stdimagefunctions.hxx>
#include <vigra/convolution.hxx>

namespace ralab{
  namespace findmf{

    /// filter LCMSImage
    /// feature finder based on gaussian smoothing and
    /// tophat morphological filter background subraction
    ///
    class LCMSImageFilter{
    protected:
      std::vector<float> signal_; //worker variable

      /// gaussian smoot image
      static void filterGauss( datastruct::LCMSImage::FloatMap::Map & mp_,
                               float scalemz = 1. , float scalert = 1.){
        //using namespace vigra::functor;
        vigra::gaussianSmoothing(vigra::srcImageRange(mp_), vigra::destImage(mp_), scalemz , scalert);
      }


      /// mexican hat smooth image using difference of gaussians
      static void diffOfGaussians( datastruct::LCMSImage::FloatMap::Map & mp_,
                                   float scalemz = 1.,
                                   float scalert = 1. )
      {
        differenceOfGaussian( vigra::srcImageRange(mp_), vigra::destImage(mp_), scalemz, scalert);
      }


      /// square map intensities
      static void sq(datastruct::LCMSImage::FloatMap::Map & mp_){
        vigra::transformImage(srcImageRange(mp_),
                              destImage(mp_),vigra::functor::sq(vigra::functor::Arg1())
                              );
      }

      /// square root intensitiess
      static void sqrt(datastruct::LCMSImage::FloatMap::Map & mp_){
        vigra::transformImage(srcImageRange(mp_),
                              destImage(mp_), vigra::functor::sqrt(vigra::functor::Arg1())
                              );
      }

      /// filter retention times
      void filterRT(datastruct::LCMSImage::FloatMap::Map & mp_, ralab::base::filter::scanfilter::IScanFilterFloatPtr  f )  {
        size_t xx = mp_.size(0);
        for( std::size_t i = 0 ; i < xx; ++i )
        {
          signal_.assign( mp_.bindInner(i).begin(), mp_.bindInner(i).end() );
          (*f)( signal_ );
          //std::replace_if( signal_.begin() , signal_.end() , std::bind2nd(std::less<float>(),0.),0.);
          std::copy(signal_.begin(),signal_.end(),mp_.bindInner(i).begin() );
        }
      }

      /// apply filter to all spectra in MZ dimension
      void filterMZ(datastruct::LCMSImage::FloatMap::Map & mp_, ralab::base::filter::scanfilter::IScanFilterFloatPtr  f ) {
        for(std::ptrdiff_t i = 0 ; i < mp_.size(1); ++i)
        {
          signal_.assign( mp_.bindOuter(i).begin(), mp_.bindOuter(i).end() );
          (*f)( signal_ );
          //std::replace_if( signal_.begin() , signal_.end() , std::bind2nd(std::less<float>(),0.),0.);
          std::copy(signal_.begin(),signal_.end(),mp_.bindOuter(i).begin());
        }
      }

    public:

      virtual void filter(datastruct::LCMSImage::FloatMap::Map & mp_, //!<
                          uint32_t mzpixelwidth, //!< in pixel
                          uint32_t rtpixelwidth, //!< in pixel
                          double mzscale=1., //!< scale
                          double rtscale=1.,
                          double factor = 1. //!< just in case some implementation needs
          )
      {
        this->filt(mp_,
                   mzpixelwidth,
                   rtpixelwidth,
                   mzscale,
                   rtscale,
                   factor);
      }

    private:
      virtual void filt(datastruct::LCMSImage::FloatMap::Map & mp_, //!<
                        uint32_t mzpixelwidth, //!< in pixel
                        uint32_t rtpixelwidth, //!< in pixel
                        double mzscale=1., //!< scale
                        double rtscale=1.,
                        double factor = 1. // size of structuring element resolution * factor);
          ) = 0;

    };

    class LCMSImageFilterMexhat : LCMSImageFilter{

      /// applies mexican hat wavelet filtering to Map
      void filt( datastruct::LCMSImage::FloatMap::Map & mp_, //!<
                 uint32_t mzpixelwidth, //!< in pixel
                 uint32_t rtpixelwidth, //!< in pixel
                 double mzscale=1., //!< scale
                 double rtscale=1.,
                 double factor = 1. // size of structuring element resolution * factor
          ) override
      {
        sqrt(mp_); // put it on nicer scale
        filterGauss(mp_,mzscale,rtscale);
        if( rtpixelwidth > 0 ){
          ralab::base::filter::scanfilter::IScanFilterFloatPtr sfpfRT =
              ralab::base::filter::scanfilter::getFilterTOPHAT(
                rtpixelwidth
                ,factor
                );
          filterRT(mp_, sfpfRT);
        }
        if( mzpixelwidth > 0 ){
          ralab::base::filter::scanfilter::IScanFilterFloatPtr sfpfMZ =
              ralab::base::filter::scanfilter::getFilterTOPHAT(
                mzpixelwidth
                ,factor
                );
          filterMZ(mp_,sfpfMZ);
        }

        //in addition filter after background removal
        diffOfGaussians( mp_ , mzscale , rtscale );
        //sq(mp_);

      }
    };


    class LCMSImageFilterGauss : LCMSImageFilter{
      /// applies gaussian smoothing, background subtraction and gaussian smoothing after
      /// background subtraction with a kernel mzscale / 2. rtscale /2. to remove
      /// artefacts due to background subtraction
      void filt( datastruct::LCMSImage::FloatMap::Map & mp_, //!<
                      uint32_t mzpixelwidth, //!< in pixel
                      uint32_t rtpixelwidth, //!< in pixel
                      double mzscale = 1., //!< scale
                      double rtscale = 1.,
                      double factor = 1. // size of structuring element resolution * factor
          ) override
      {
        std::cout << "mzscale: " << mzscale << " rtscale: " << rtscale
                  << " mzw: " << mzpixelwidth << " rt: " << rtpixelwidth
                  <<  " factor : " << factor << std::endl;
        sqrt(mp_); // put it on nicer scale
        filterGauss(mp_,mzscale,rtscale);
        if( rtpixelwidth > 0 ){
          ralab::base::filter::scanfilter::IScanFilterFloatPtr sfpfRT =
              ralab::base::filter::scanfilter::getFilterTOPHAT(
                rtpixelwidth
                ,factor
                );
          filterRT(mp_,sfpfRT);
        }
        if( mzpixelwidth > 0 ){
          ralab::base::filter::scanfilter::IScanFilterFloatPtr sfpfMZ =
              ralab::base::filter::scanfilter::getFilterTOPHAT(
                mzpixelwidth
                ,factor
                );
          filterMZ(mp_,sfpfMZ);
        }

        //to remove background subtraction artifacts
        filterGauss( mp_ , mzscale/2. , rtscale/2. );
        sq(mp_);
        ///mp_.updateImageMax();
      }
    };


  }
}//end namespace
#endif // LCMSIMAGEFILTER_H
