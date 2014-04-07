// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef FEATUREFINDER_H
#define FEATUREFINDER_H

#include <boost/filesystem.hpp>
#include <vigra/multi_array.hxx>
#include <vigra/multi_localminmax.hxx>
#include <vigra/seededregiongrowing.hxx>


//#include "datastruct/ifeatureaccess.h"
#include "findmf/datastruct/lcmsimage.h"

namespace ralab{
  namespace findmf{
    template <class T>
    struct EqualWithToleranceFunctor
    {
      EqualWithToleranceFunctor(T tolerance)
        : t(tolerance)
      {}

      bool operator()(T l, T r) const
      {
        return vigra::abs(l-r) <= t;
      }
      T t;
    };


    /// because the seeded region growing assumes local minima and we have local maxima we flip the image
    /// at the same time we do thresholding (setting values less than threshold to 0).
    template <class T>
    struct ImageTransformatorFlip
    {
      ImageTransformatorFlip(T threshold, T mmax)
        : thresh_(threshold), max_(mmax-thresh_)
      {}

      template<class SrcValueTyp>
      T operator()(SrcValueTyp const & val) const
      {
        T a = val - thresh_; //subtract threshold
        a = 0 > a ? 0 : a; // set negative values to 0
        // they will not be assigned to segements .
        a = (-1*a) + max_; //flip an move above 0

        return vigra::NumericTraits<T>::fromRealPromote(a);
      }

      T thresh_;
      T max_;
    };

    /// after computing the segment boundaries we flip the image back so that we can use it further
    /// to compute feature statistics
    template <class T>
    struct ImageTransformatorFlipBack
    {
      ImageTransformatorFlipBack(T mmax , T threshold)
        :  max_(mmax) , thresh_(threshold)
      {}
      template<class SrcValueTyp>
      T operator()(SrcValueTyp const & val) const
      {
        T a = (-1*val) + max_; //flip an move above 0
        a = val > 0 ? a + thresh_ : a; // add threshold back
        return vigra::NumericTraits<T>::fromRealPromote(a);
      }

      T max_;
      T thresh_;
    };


    /// Image segementation part
    class FeatureFinder
    {
    public:
      typedef vigra::MultiArray< 2 , int > Labels;
      typedef vigra::MultiArray< 2 , float > Gradient;

    protected:
      Labels labels_;
      int backgroundLabel_;

    public:
      virtual ~FeatureFinder(){}

      /// access to label image
      Labels & getLabels(){
        return labels_;
      }

      /// executes findFeature, createFeatures and extractFeatrues
      void findFeature(Gradient & gradient, double threshold){
        this->findFeat(gradient, threshold);
      }

    private:
      /// private abstract findFeats
      virtual void findFeat(Gradient & data, //[inout]
                            float threshold
                            ) = 0;
    };


    ///
    class FeatureFinderLocalMax : public  FeatureFinder{
      //method to find features

    private:
      void findFeat(Gradient & data, //[inout]
                    float threshold
                    ){
        using namespace vigra;
        Gradient & gradient_ = data;//you work with the data
        vigra::FindMinMax<Gradient::value_type> minmax;   // functor to find range
        inspectImage(srcImageRange(gradient_), minmax); // find original range
        //LOG(INFO)<< " >>> attempt reshape of labels <<< ";
        labels_.reshape(gradient_.shape());

        // flip image - because seededregiongrowin expects local minima
        // just flip the image so you can run seeded region growing.
        transformImage( srcImageRange(gradient_) , destImage(gradient_),
                        ImageTransformatorFlip<float>( 0. , minmax.max )
                        );
        vigra::localMinima(srcImageRange(gradient_), destImage(labels_),
                           vigra::LocalMinmaxOptions().neighborhood(4).allowAtBorder());
        int max_region_label = labelImageWithBackground(srcImageRange(labels_), destImage(labels_), false, 0);

        // create a statistics functor for region growing
        vigra::ArrayOfRegionStatistics< vigra::SeedRgDirectValueFunctor<float> > gradstat( max_region_label ) ;


        seededRegionGrowing(srcImageRange(gradient_), srcImage(labels_),
                            destImage(labels_),
                            gradstat,
                            //SRGType(StopAtThreshold | KeepContours),
                            SRGType(StopAtThreshold),// | KeepContours),
                            FourNeighborCode(),
                            static_cast<double>(minmax.max - (threshold + 0.001))
                            );

        // flip the image back.
        transformImage( srcImageRange(gradient_) , destImage(gradient_),
                        ImageTransformatorFlipBack<float>(  minmax.max , 0. )
                        );
      }//end
    };


    //flip back image
    class FeatureFinderMexhat : public FeatureFinder{
      //method to find features
      void findFeat(Gradient & data, //!< [inout]
                    float threshold = 0. //!< threshold for local maxima search in 2. derivative.
                    ) override {
        using namespace vigra;
        Gradient & gradient_ = data;//you work with the data
        vigra::FindMinMax<Gradient::value_type> minmax;   // functor to find range
        inspectImage(srcImageRange(gradient_), minmax); // find original range
        //LOG(INFO)<< " >>> attempt reshape of labels <<< ";
        labels_.reshape(gradient_.shape());

        vigra::localMaxima( srcImageRange(gradient_), destImage(labels_),
                            vigra::LocalMinmaxOptions().neighborhood(4).allowAtBorder().threshold(threshold)
                            );
        int max_region_label = labelImageWithBackground(srcImageRange(labels_), destImage(labels_), false, 0 );

        // flip image since seeded region requires minima
        transformImage( srcImageRange(gradient_) , destImage(gradient_), boost::bind(std::multiplies<float>(), _1, -1.));

        // create a statistics functor for region growing
        vigra::ArrayOfRegionStatistics< vigra::SeedRgDirectValueFunctor<float> > gradstat( max_region_label ) ;

        seededRegionGrowing(srcImageRange(gradient_), srcImage(labels_),
                            destImage(labels_),
                            gradstat
                            //SRGType(StopAtThreshold | KeepContours),
                            ,SRGType(StopAtThreshold),// | KeepContours),
                            FourNeighborCode(),
                            0.001
                            //,static_cast<double>(minmax.max - (threshold + 0.001))
                            );

        //T ODO check if you really need it?
        // flip the image back
        transformImage( srcImageRange(gradient_) , destImage(gradient_), boost::bind(std::multiplies<float>(), _1, -1.));
      }
    };



  }
}

#endif // FEATUREFINDER_H
