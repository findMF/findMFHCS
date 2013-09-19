// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT




#ifndef FEATUREPRINTER_H
#define FEATUREPRINTER_H

#include <ostream>
//#include "datastruct/ifeatureaccess.h"
#include "findmf/datastruct/featuresmap.h"

namespace ralab{

  struct FeaturesMapPrinter {
    //std::ostream & stream_;
    std::string sep_;


    FeaturesMapPrinter( const std::string & sep="\t"): sep_(sep){

    }

    void writeFeatures(const std::string & output_directory,
                       const std::string & outfileprefix,
                       FeaturesMap & map
                       ){
      std::fstream featureAcc;
      boost::filesystem::path p(output_directory);
      // write feature table
      {
          std::string outfile1 = outfileprefix;
          outfile1 +="feature.csv";

          boost::filesystem::path p1 = p;
          p1 /= outfile1;
          featureAcc.open(p1.string().c_str() , std::fstream::out);

          printHeader(featureAcc);

          //iterate over feature map.
          FeaturesMap::Features::iterator beg, end;
          beg = map.features().begin();
          end = map.features().end();
          for(int i = 0;beg != end; ++beg, ++i){
              FeatureReadAdapterFM x(*beg, map.getMapDescription());
              visit( x , featureAcc);
            }
          featureAcc.close();
      }
    }

  private:
    template<typename value_type>
    std::ostream & projection2Stream(std::vector<value_type> & projection, std::iostream & stream_)
    {
      typename std::vector<value_type>::iterator beg,end;
      end= projection.end();
      stream_ << "[";
      for(beg = projection.begin(); beg !=  end; ++beg){
          stream_<<*beg<<"|";
        }
      stream_ << "]";
      return stream_;
    }

    std::ostream & printHeader(std::iostream & stream_){
      stream_ << "centerOfMassMZ" << sep_;
      stream_ << "centerOfMassRT" << sep_;
      stream_ << "MZ" << sep_;
      stream_ << "RT" << sep_;
      stream_ << "MZSD" << sep_;
      stream_ << "RTSD" << sep_;
      stream_ << "MZSKEW" << sep_;
      stream_ << "RTSKEW" << sep_;
      stream_ << "MZKURT" << sep_;
      stream_ << "RTKURT" << sep_;
      stream_ << "Max" << sep_;
      stream_ << "Count" << sep_;
      stream_ << "Volume" << sep_;
      stream_ << "maximumLocationMZ" << sep_;
      stream_ << "maximumLocationRT" << sep_;
      stream_ << "minMZIndex" << sep_;
      stream_ << "mzExtend" << sep_;
      stream_ << "minRTIndex" << sep_;
      stream_ << "rtExtend" << sep_;
      stream_ << "mzProjection" << sep_;
      stream_ << "rtProjection" << sep_;
      stream_ << std::endl;
      return stream_;
    }

  public:

    void visit(ralab::IFeatureAllAccess & feature, std::iostream & stream_) override {
      stream_ << feature.getCenterOfMassMZ() << sep_;
      stream_ << feature.getCenterOfMassRT() << sep_;

      stream_ << feature.getApexMZ() << sep_;
      stream_ << feature.getApexRT() << sep_;

      stream_ << feature.getSDMZ() << sep_;
      stream_ << feature.getSDRT() << sep_;
      stream_ << feature.getKurtosisMZ() << sep_;
      stream_ << feature.getKurtosisRT() << sep_;
      stream_ << feature.getSkewnessMZ() << sep_;
      stream_ << feature.getSkewnessRT() << sep_;

      stream_ << feature.getMaximum() << sep_;
      stream_ << feature.getCount() << sep_;
      stream_ << feature.getVolume() << sep_;
      stream_ << feature.getMaxLocationMZ() << sep_;
      stream_ << feature.getMaxLocationRT() << sep_;
      stream_ << feature.getMinMZIdx() << sep_;
      stream_ << feature.getMZExtend() << sep_;
      stream_ << feature.getMinRTIdx() << sep_;
      stream_ << feature.getRTExtend() << sep_;

      projection2Stream(feature.getMZProjection(), stream_ ) ;
      stream_ << sep_;
      projection2Stream(feature.getRTProjection(), stream_ );
      stream_ << std::endl;
    }
  };
}
#endif // FEATUREPRINTER_H
