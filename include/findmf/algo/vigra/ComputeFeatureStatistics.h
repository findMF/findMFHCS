#ifndef COMPUTEFEATURESTATISTICS_H
#define COMPUTEFEATURESTATISTICS_H

#include "vigratypedefs.h"
#include "findmf/datastruct/featuresmap.h"

namespace ralab{
  namespace findmf{
    namespace vigac = vigra::acc;


    /// compute feature statistics
    struct ComputeFeatureStatistics{
      typedef vigra::MultiArray< 2 , int > Labels;
      typedef vigra::MultiArray< 2 , float > Gradient;

    private:

      ralab::findmf::MyAccumulatorChain mac_;

    public:
      /// creates feature statistics
      void extractFeatures(datastruct::FeaturesMap & features,  const Gradient & gradient,const Labels & labels){
        this->extractFeatureChain(gradient , labels, mac_);
        // generate features from statistics " ;
        this->createFeaturesFromStatistics( features, mac_);
        // create intensity projections ;
        this->createIntensityProjections( gradient , labels, features );
        // create projection stats ;
        this->creatProjectionStats(features);
      }

    private:
      //method to extract the features
      void extractFeatureChain(const Gradient & data, //!< the data
                               const Labels & labels, //!< the labels
                               ralab::findmf::MyAccumulatorChain & acummulatorChain)
      {
        //Gradient weights;
        //weights.reshape(Gradient::difference_type(data.size(0),data.size(1)));
        //vigra::copyMultiArray(vigra::srcMultiArrayRange(data),vigra::destMultiArrayRange(weights));

        ralab::findmf::Iterator start = createCoupledIterator(data, labels);//, data);
        ralab::findmf::Iterator end = start.getEndIterator();

        //don't analyse background.
        acummulatorChain.ignoreLabel(0);
        //acummulatorChain.ignoreLabel(0); //statistics will not be computed for region 0 (e.g. background)
        vigra::acc::extractFeatures(start,end,acummulatorChain);
      }

      void creatProjectionStats(ralab::findmf::datastruct::FeaturesMap & map){
        ralab::findmf::datastruct::FeaturesMap::Features::iterator beg , end ;
        ralab::findmf::utilities::PickApex picker;
        int count = 0;
        end = map.features().end();
        for( beg = map.features().begin() ; beg != end ; ++beg ){
          ralab::findmf::datastruct::Feature2D &x = *beg;
          ralab::findmf::utilities::computeStats(x.getProjectionMZ(),x.getMinMZIdx(),x.getMZStats());
          x.getMZStats().peaklockation_ = picker.pick(x.getProjectionMZ(),x.getMZStats());
          x.setProblem(picker.problem_);
          ralab::findmf::utilities::computeStats(x.getProjectionRT(),x.getMinRTIdx(),x.getRTStats());
          x.getRTStats().peaklockation_ = picker.pick(x.getProjectionRT(),x.getRTStats());
          x.setProblem(picker.problem_);
          ++count;
        }
        //LOG(INFO) << "picking done";
      }


      //copy statistics generated in accumulator chain into features
      static void createFeaturesFromStatistics( datastruct::FeaturesMap & features ,
                                                findmf::MyAccumulatorChain & chain
                                                )
      {
        using namespace vigra;
        using namespace vigra::acc;
        int lengthAC = chain.regionCount();

        //DLOG(INFO) << " Length of accumulator chain : " << lengthAC ;

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
      void createIntensityProjections( const Gradient & data , const Labels & labels, datastruct::FeaturesMap & mf )
      {
        typedef vigra::CoupledIteratorType<2, float, int>::type Iterator;
        Iterator beg = createCoupledIterator(data, labels);
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

      /// prints accumulator chain
      std::ostream &  printFAcc(
          std::ostream & stream ,
          ralab::findmf::MyAccumulatorChain & acummulatorChain
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

      // Print the accumulator chain
      void writeFeatures(const std::string & output_directory,
                         const std::string & outfileprefix)
      {
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
  }//end namespace
}//end namespace

#endif // COMPUTEFEATURESTATISTICS_H
