// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include <QApplication>
#include <iostream>
#include <boost/timer.hpp>
#include <boost/thread.hpp>

#include <tbb/parallel_for_each.h>
#include <tbb/task_scheduler_init.h>

#include <glog/logging.h>

#include "algo/filter.h"

#include "fileio/spectramap.h"

#include "application/swathprocessor.h"
#include "application/featurefinderapp.h"

//#include "src/qtwidgets/TwoPanelImageWidget.h"
#include "fileio/helperfunctions.h"
#include "parseargExtract.h"


namespace ralab{

  //process single swath or ms map.
  struct SwathProcessor{

    struct Params{
      std::string infile;
      std::string outdir;
      double resolution; // with of mz bins in ppms
      uint32_t rtresolution; // retention time resolution
      uint32_t mzresolution; // in pixel
      double minmass; // minimal mass to consider
    };


    std::vector<std::size_t> keys;
    int i_;

    Params anap_;
    ralab::SwathInfoPtr sip_;
    std::string filename_;
    std::string filestem_;
    boost::filesystem::path outdir_;

    SwathProcessor( int i ,//which swath to process
                    const SwathProcessor::Params & ap, // the analysis parameter
                    ralab::SwathInfoPtr sip // swath info pointer
                    )
      :sip_(sip), i_(i), anap_(ap)
    {
      prepareOutputFile();
    }

    //stern...
    void prepareOutputFile()
    {
      if( !boost::filesystem::exists(anap_.infile) )
        {
          LOG(ERROR) << "could not find specified file :" ;
          LOG(ERROR) << anap_.infile << std::endl;
          return;
        }
      //boost::filesystem::path p1 = createOutputs(anap_.infile,boost::lexical_cast<std::string>(i_));
      filestem_ = boost::filesystem::path(anap_.infile).stem().string(); //createOutputs(p1.string() ,"fitered");
      outdir_ = boost::filesystem::path(anap_.outdir);
      outdir_ /= filestem_;
      boost::filesystem3::create_directory(outdir_);

      filestem_ += boost::lexical_cast<std::string>(i_);
    }

    //test ...
    void operator()(void) {
      //LOG(INFO) << "start processing swath :" << i_;
      sip_->getKeys(keys);
      pwiz::msdata::MSDataPtr msdataptr = pwiz::msdata::MSDataPtr(new pwiz::msdata::MSDataFile(anap_.infile));
      ralab::SpectraMap tmp (msdataptr,sip_ , anap_.resolution );
      LOG(INFO) << "processing map : " << i_ << " with key : " << keys[i_] ;
      ralab::MapLCMSProcessorPtr mp = tmp.getMap( keys[i_] ,anap_.minmass );

      boost::timer time;
      //start the computational intensive task...
      mp->filterMap( anap_.mzresolution , anap_.rtresolution );

      boost::filesystem::path x = outdir_ / filestem_ ;
      mp->write_image( x.string());

      LOG(INFO) << "map i : " << i_ << " read and filtered : " << time.elapsed() ;
      {
        ralab::FeatureFinderApp ff;
        time.restart();
        ff.findFeature( mp->getMap(), 10. );
        ff.setMapDescription(mp->getMapDescription());
        LOG(INFO) << "swath i : " << i_ << " computed segments and feature statistics : " << time.elapsed() << std::endl;
        ff.writeFeatures( outdir_.string() , filestem_ );
        ff.writeFeatures2SQL( outdir_.string() , filestem_ );
        LOG(INFO) << "written features : " << time.elapsed() ;
      }
      LOG(INFO) << "map " << i_ << " processed." ;
    }
  };
}//end namespace ralab


template <typename T> struct invoker {
  void operator()(T& it) const {it();}
};


int main(int argc, char *argv[])
{
  google::InitGoogleLogging(argv[0]);
  google::LogToStderr();

  // do some testing first.
  b_po::variables_map vmgeneral;
  parsecommandlineExtract(argc, argv, vmgeneral);

  AnalysisParameters ap;
  analysisParameters(ap,vmgeneral);

  boost::timer time;
  LOG(INFO) << "start reading properties" << std::endl;
  pwiz::msdata::MSDataPtr msdataptr = pwiz::msdata::MSDataPtr(new pwiz::msdata::MSDataFile(ap.infile));
  LOG(INFO)  << "time to open file " << time.elapsed() << std::endl;

  ralab::SwathPropertiesReader swathPropReader(msdataptr);
  std::vector<std::size_t> keys;
  swathPropReader.getSwathInfo()->getKeys(keys);

  LOG(INFO)  << "time required to scan through file :" << time.elapsed() << std::endl;

  ralab::SwathProcessor::Params pars;
  pars.infile = ap.infile;
  pars.minmass = ap.minmass;
  pars.outdir = ap.outdir;
  pars.mzresolution = ap.mzresolution;
  pars.rtresolution = ap.rtresolution;
  pars.resolution = ap.resolution;


  tbb::task_scheduler_init init(ap.nrthreads);  // Automatic number of threads
  std::vector<ralab::SwathProcessor> tasks;
  for(int i = 0 ; i < keys.size() ; ++i){
      tasks.push_back(ralab::SwathProcessor(i , pars , swathPropReader.getSwathInfo() ));
    }
  tbb::parallel_for_each(tasks.begin(),tasks.end(),invoker<ralab::SwathProcessor>());

  return 0;
}



