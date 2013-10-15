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
#include <glog/logging.h>


#include "vigratypedefs.h"
//#include "datastruct/ifeatureaccess.h"
#include "findmf/datastruct/featuresmap.h"
#include "findmf/datastruct/lcmsimage.h"

namespace ralab{
  namespace findmf{
    namespace vigac = vigra::acc;
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


    //
    //feature finder
    //
    class FeatureFinder
    {
    public:
      typedef vigra::MultiArray< 2 , int > Labels;
      typedef vigra::MultiArray< 2 , float > Gradient;

    protected:

      Labels labels_;
      int backgroundLabel_;
      ralab::featurefind::MyAccumulatorChain mac_;


    public:

      Labels & getLabels(){
        return labels_;
      }

      /* executes findFeature, createFeatures and extractFeatrues */
      void findFeature(Gradient & gradient, double threshold){
        this->findFeat(gradient, threshold);
        this->extractFeatureChain(gradient,mac_);
      }

      void extractFeatures(datastruct::FeaturesMap & features, const Gradient & gradient){
        LOG(INFO)<< " >>>> go for statistics " ;
        this->createFeaturesFromStatistics( features, mac_);
        LOG(INFO)<< " >>>> created features statistics " ;
        this->extractFeaturesI( gradient , features );
        LOG(INFO)<< " >>>> create projection stats " ;
        this->creatProjectionStats(features);
        LOG(INFO)<< " >>>> done " ;

      }

    private:
      void creatProjectionStats(ralab::findmf::datastruct::FeaturesMap & map){
        ralab::findmf::datastruct::FeaturesMap::Features::iterator beg , end ;
        ralab::findmf::utilities::Pick picker;
        try{
          int count = 0;
          end = map.features().end();
          for( beg = map.features().begin() ; beg != end ; ++beg ){
              ralab::findmf::datastruct::Feature2D &x = *beg;
              ralab::findmf::utilities::computeStats(x.getProjectionMZ(),x.getMinMZIdx(),x.getMZStats());
              x.getMZStats().peaklockation_ = picker.pick(x.getProjectionMZ(),x.getMZStats());
              ralab::findmf::utilities::computeStats(x.getProjectionRT(),x.getMinRTIdx(),x.getRTStats());
              x.getRTStats().peaklockation_ = picker.pick(x.getProjectionRT(),x.getRTStats());
              ++count;
              //std::cout << map.features().size() <<" " << count << " " << x.getProjectionMZ().size() << " " << x.getProjectionRT().size() << std::endl;
            }
        }catch(std::exception &e){
          LOG(INFO) << e.what();
        }

        LOG(INFO) << "picking done";
      }

      //method to extract the features
      void extractFeatureChain(Gradient & data,
                               ralab::featurefind::MyAccumulatorChain & acummulatorChain)
      {
        Gradient weights;
        weights.reshape(Gradient::difference_type(data.size(0),data.size(1)));
        vigra::copyMultiArray(vigra::srcMultiArrayRange(data),vigra::destMultiArrayRange(weights));
        /* vigra::transformImage(vigra::srcImageRange(data) ,
                            vigra::destImage(weights) , vigra::functor::sq(vigra::functor::Arg1())
                            ); */
        ralab::featurefind::Iterator start = createCoupledIterator(data, labels_, weights);
        ralab::featurefind::Iterator end = start.getEndIterator();
        //don't analyse background.
        acummulatorChain.ignoreLabel(0);
        //acummulatorChain.ignoreLabel(0); //statistics will not be computed for region 0 (e.g. background)
        vigra::acc::extractFeatures(start,end,acummulatorChain);
      }


      //copy statistics generated in accumulator chain into features
      static void createFeaturesFromStatistics( datastruct::FeaturesMap & features ,
                                                featurefind::MyAccumulatorChain & chain
                                                )
      {
        using namespace vigra;
        using namespace vigra::acc;
        int lengthAC = chain.regionCount();

        DLOG(INFO) << " Length of accumulator chain " << lengthAC ;

        --lengthAC; //decrease because you are skipping the background...
        features.resize(lengthAC);
        for(int i=0; i < lengthAC; ++i)
          {
            int j = i + 1; // ignore background
            TinyVector<int, 2> blaImax = get< Coord< Maximum> >(chain, j);
            TinyVector<int, 2> blaImin = get< Coord< Minimum > >(chain, j);
            int xsize = blaImax[0] - blaImin[0] + 1;
            int ysize = blaImax[1] - blaImin[1] + 1;
            features.features()[i] = findmf::datastruct::Feature2D(
                  blaImin[0],
                xsize,
                blaImin[1],
                ysize
                );
            float val = vigac::get< Sum >(chain, j);
            features.at(i).setVolume(val);
            val =vigac::get< Maximum >(chain, j);
            features.at(i).setMaximum(val);
            int count =vigac::get< Count >(chain, j);
            features.at(i).setCount(count);
            //get Center Of Mass
            TinyVector<float, 2> tvFloat = vigac::get<CenterOfMass>(chain, j);
            double mz =tvFloat[0];
            double rt =tvFloat[1];

            features.at(i).setCenterOfMassMZ(mz);
            features.at(i).setCenterOfMassRT(rt);
            tvFloat = vigac::get< Coord< ArgMaxWeight > >(chain, j);

            mz =( tvFloat[0] );
            rt =( tvFloat[1] );

            features.at( i ).setMaxLocationMZ(mz);
            features.at( i ).setMaxLocationRT(rt);
            features.at( i ).setID(i);
          }
      }


      //create intensity projections in RT and MZ
      void extractFeaturesI( const Gradient & data , datastruct::FeaturesMap & mf )
      {
        typedef vigra::CoupledIteratorType<2, float, int>::type Iterator;
        Iterator beg = createCoupledIterator(data, labels_);
        Iterator end = beg.getEndIterator();

        for (Iterator it = beg; it < end; ++it) {
            int label = it.get<2>();
            if(label > 0){ //exlude the background
                double intensity = it.get<1>();
                vigra::TinyVector<int, 2> coor = it.get<0>();
                mf.at(label-1).add(coor[0],coor[1],intensity);
              }
          }
      }

    private:
      std::ostream &  printFAcc(
          std::ostream & stream ,
          ralab::featurefind::MyAccumulatorChain & acummulatorChain
          ){
        using namespace vigac;

        //create header
        stream << "index\t Count \t Mean \t Variance \t Sum \t Maximum \t";
        stream << "RegionCenterX \t RegionCenterY \t";
        stream << "CenterOfMassX \t CenterOfMassY \t";
        stream << "c_minimumX \t c_minimumY \t";
        stream << "c_maximumX \t c_maximumY \t";

        //stream << "UnbiasedSkewness \t";// UnbiasedSkewnessU\t";
        //stream << "UnbiasedKurtosis \t";// UnbiasedKurtosisU\t";
        //stream << "StdDevX \t";// StdDevY";
        //stream << "RegionRadii1 \t";
        //stream << "RegionRadii2 \t";
        //stream << "Moi1 \t";
        //stream << "Moi2 \t";

        stream << std::endl;

        //stream << "\t" << get<Global< Mean > >( acummulatorChain);
        //stream << "\t" << get<Global< Variance > >( acummulatorChain);
        //      double x =  get< Global< Mean > >( acummulatorChain);

        int xxx = acummulatorChain.regionCount();
        for(int i=0; i < xxx; ++i)
          {
            stream << i ;
            stream << "\t" << get< Count >(acummulatorChain, i);
            stream << "\t" << get< Mean >(acummulatorChain, i);
            stream << "\t" << get< Variance >(acummulatorChain, i);
            stream << "\t" << get< Sum >(acummulatorChain,i);
            stream << "\t" << get< Maximum >(acummulatorChain, i);

            //Region Center
            vigra::TinyVector<double, 2> bla = get< RegionCenter >(acummulatorChain,i);
            stream << "\t" << bla[0];
            stream << "\t" << bla[1];
            //stream << "\t" << get< RegionCenter >(acummulatorChain, i);

            //Center Of mass
            bla = get< CenterOfMass >(acummulatorChain,i);
            stream << "\t" << bla[0];
            stream << "\t" << bla[1];

            //Minimum Coordinate
            vigra::TinyVector<int, 2> blaI = get< Coord< Minimum> >(acummulatorChain, i);
            stream << "\t" << blaI[0];
            stream << "\t" << blaI[1];
            //stream << "\t" << get< Coord< Minimum > >(acummulatorChain, i);

            //Maximum Coordinate
            blaI = get< Coord< Maximum> >(acummulatorChain, i);
            stream << "\t" << blaI[0];
            stream << "\t" << blaI[1];

            //  stream << "\t" << get<UnbiasedSkewness >( acummulatorChain , i );
            //  stream << "\t" << get<UnbiasedKurtosis>( acummulatorChain , i );
            //  stream << "\t" << get<StdDev>( acummulatorChain , i );
            //  stream << "\t" << get<Weighted<StdDev> >( acummulatorChain , i );
            //  stream << "\t" << get<Weighted< < StdDev> > >( acummulatorChain , i );
            //  ralab::featurefind::RegionRadiiReturn rrad = get<RegionRadii>(acummulatorChain, i);
            //  ralab::featurefind::MomentsOfInertiaReturn moi = get<MomentsOfInertia>(acummulatorChain,i);
            //   stream << "\t" <<rrad[0];
            //   stream << "\t" <<rrad[1];
            //   stream << "\t" << moi[0];
            //   stream << "\t" << moi[1];
            //AxesOfInertiaReturn moi = get<AxesOfInertia>(accumulatorChain,i);
            //stream << "\t" << blaI[0];
            //stream << "\t" << blaI[1];
            stream << std::endl;
          }
        return stream;
      }


    public:
      /*prints feature accumulator chain */
      std::ostream & printFeaturesAcc(std::ostream & statsfile2){
        return this->printFAcc(statsfile2,  mac_ );
      }


      //method to find features
      void findFeat(Gradient & data, //[inout]
                    float threshold,
                    float tolerance = 2.//minima tolerance
          ){
        using namespace vigra;
        Gradient & gradient_ = data;//you work with the data
        vigra::FindMinMax<Gradient::value_type> minmax;   // functor to find range
        inspectImage(srcImageRange(gradient_), minmax); // find original range
        LOG(INFO)<< " >>> attempt reshape of labels <<< ";
        labels_.reshape(gradient_.shape());
        LOG(INFO) << " >>> have labels  ";

        if(0){
            transformImage( srcImageRange(gradient_) , destImage(gradient_),
                            vigra::linearIntensityTransform(
                              -1., // scaling this flips the image
                              -( minmax.max ) ) //
                            );

            //flip image
          }else{
            transformImage( srcImageRange(gradient_) , destImage(gradient_),
                            ImageTransformatorFlip<float>( threshold , minmax.max )
                            );
          }
        //vigra::LocalMinmaxOptions lmx;
        //lmx.markWith(1).allowPlateaus().thresh(10.);
        extendedLocalMinima(srcImageRange(gradient_),
                            destImage(labels_),
                            1,
                            FourNeighborCode(),
                            //EightNeighborCode(),
                            EqualWithToleranceFunctor<float>(tolerance)
                            );

        int max_region_label = labelImageWithBackground(srcImageRange(labels_), destImage(labels_), false, 0);
        // create a statistics functor for region growing
        vigra::ArrayOfRegionStatistics< vigra::SeedRgDirectValueFunctor<float> > gradstat( max_region_label ) ;

        // perform region growing, starting from the minima of the gradient magnitude;
        // as the feature (first input) image contains the gradient magnitude,
        // this calculates the catchment basin of each minimum
        seededRegionGrowing(srcImageRange(gradient_), srcImage(labels_),
                            destImage(labels_),
                            gradstat,
                            //SRGType(StopAtThreshold | KeepContours),
                            SRGType(StopAtThreshold),// | KeepContours),
                            FourNeighborCode(),
                            static_cast<double>(minmax.max - (threshold + 0.001))
                            );
        transformImage( srcImageRange(gradient_) , destImage(gradient_),
                        ImageTransformatorFlipBack<float>(  minmax.max , threshold )
                        );
        LOG(INFO) << " >>> segments done  ";

      }

      //Write the acc chain into csv
      void writeFeatures(const std::string & output_directory,
                         const std::string & outfileprefix){
        std::fstream featureAcc;
        boost::filesystem::path p(output_directory);

        // write
        {
          std::string outfile1 = outfileprefix;
          outfile1 +=".csv";

          boost::filesystem::path p1 = p;
          p1 /= outfile1;
          std::string xx = p1.string();
          featureAcc.open(xx.c_str(), std::fstream::out);

          this->printFeaturesAcc(featureAcc);
          featureAcc.close();
        }
      }


    };
  }
}

#endif // FEATUREFINDER_H
