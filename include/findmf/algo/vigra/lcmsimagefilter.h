// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef LCMSIMAGEFILTER_H
#define LCMSIMAGEFILTER_H

#include "findmf/datastruct/lcmsimage.h"
#include "findmf/algo/vigra/mexhat2DNaive.h"
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
    private:
      std::vector<float> signal_; //worker variable

      /// gaussian smoot image
      static void filterGauss( datastruct::LCMSImage::FloatMap::Map & mp_,
                               float scalemz = 1. , float scalert = 1.){
        //using namespace vigra::functor;
        vigra::gaussianSmoothing(vigra::srcImageRange(mp_), vigra::destImage(mp_), scalemz , scalert);
      }

      /// mexican hat smooth image
      static void filterMexicanHat( datastruct::LCMSImage::FloatMap::Map & mp_,
                                    float scalemz = 1. , float scalert = 1.){
        //using namespace vigra::functor;

        gaussianSmoothing(vigra::srcImageRange(mp_), vigra::destImage(mp_), scalemz, scalert);
        // define horizontal Sobel filter
        vigra::Kernel2D<float> laplace2d;
        // upper left and lower right

        laplace2d.initExplicitly(vigra::Diff2D(-1,-1), vigra::Diff2D(1,1)) =
            0.  ,  -1., 0.,
            -1. ,  4., -1.,
            0.  , -1., -0.;
        laplace2d.setBorderTreatment(vigra::BORDER_TREATMENT_REFLECT);
        //vigra::convolveImage(vigra::srcImageRange(mp_), vigra::destImage(mp_), vigra::kernel2d(laplace2d));
        //vigra::mexicanHat2D(vigra::srcImageRange(mp_), vigra::destImage(mp_), (double) scalemz, (double) scalert );
        //vigra::laplacianOfGaussian(vigra::srcImageRange(mp_), vigra::destImage(mp_), scalemz );
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

      /// applies mexican hat wavelet filtering to Map
      void filterMapMexHat( datastruct::LCMSImage::FloatMap::Map & mp_, //!<
                            uint32_t mzpixelwidth, //!< in pixel
                            uint32_t rtpixelwidth, //!< in pixel
                            double mzscale=1., //!< scale
                            double rtscale=1.,
                            double factor = 1. // size of structuring element resolution * factor
          )
      {
        sqrt(mp_); // put it on nicer scale
        /*filterGauss(mp_,mzscale,rtscale);
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
        }*/

        //in addition filter after background removal
        filterMexicanHat( mp_ , mzscale , rtscale );
        //sq(mp_);
        //mp_.updateImageMax();
      }


      /// applies gaussian smoothing, background subtraction and gaussian smoothing after
      /// background subtraction with a kernel mzscale / 2. rtscale /2. to remove
      /// artefacts due to background subtraction
      void filterMap( datastruct::LCMSImage::FloatMap::Map & mp_, //!<
                      uint32_t mzpixelwidth, //!< in pixel
                      uint32_t rtpixelwidth, //!< in pixel
                      double mzscale = 1., //!< scale
                      double rtscale = 1.,
                      double factor = 1. // size of structuring element resolution * factor
          )
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

        //in addition filter after background removal
        filterMexicanHat( mp_ , mzscale/2. , rtscale/2. );
        sq(mp_);
        //mp_.updateImageMax();
      }
    };


  }
}//end namespace
#endif // LCMSIMAGEFILTER_H
