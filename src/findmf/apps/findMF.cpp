// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include <QApplication>
#include <iostream>
#include <boost/timer.hpp>
#include <boost/thread.hpp>

#include <tbb/task_scheduler_init.h>
#include <tbb/pipeline.h>
#include <tbb/concurrent_queue.h>

#include <glog/logging.h>

#include "findmf/application/lcmsimagereader.h"
#include "findmf/algo/vigra/featurefinder.h"

#include "findmf/fileio/helperfunctions.h"
#include "parseargExtract.h"

#include "findmf/fileio/sqlite2/featuresmapsqlwriterfacade.h"
#include "findmf/application/lcmsimagefilter.h"

#include "toolparameters.h"

namespace ralab
{


  //process single swath or ms map
  struct LCMSImageReaderFilter : tbb::filter{
    std::vector<std::size_t> keys;
    ralab::SwathInfoPtr sip_;
    Params anap_;
    uint32_t count_;

    LCMSImageReaderFilter(
        const Params & ap, // the analysis parameter
        ralab::SwathInfoPtr sip // swath info pointer
        ) :tbb::filter(serial_in_order),sip_(sip), anap_(ap),count_(0)
    {
      anap_.prepareOutputFile(false);
    }
    //test ...
    void * operator()(void * )
    {
      LOG(INFO) << "start reading" ;
      boost::timer time;
      time.restart();
      std::vector< std::size_t > keys;
      sip_->getKeys(keys);
      if(count_ < keys.size())
        {

          ralab::LCMSImage * mp = new ralab::LCMSImage();
          {
            pwiz::msdata::MSDataPtr msdataptr = pwiz::msdata::MSDataPtr(new pwiz::msdata::MSDataFile(anap_.infile));
            ralab::LCMSImageReader tmp (msdataptr,sip_ , anap_.ppm , anap_.rt2sum_);
            LOG(INFO) << "processing map : " << count_ << " with key : " << keys[count_] ;
            tmp.getMap( keys[count_] ,anap_.minmass , anap_.maxmass, *mp);
            count_++;
          }
          LOG(INFO) << "Reading done : " << time.elapsed() << std::endl;
          return mp;
        }
      else
        {
          return NULL;
        }
      
    }
  };
  //end LCMSImageReader

  //second stage filtering...
  struct MapFilter:tbb::filter{
    uint32_t mzpixelwidth_;
    uint32_t rtpixelwidth_;
    double mzscale_;
    double rtscale_;


    MapFilter(uint32_t mzpixwidth, uint32_t rtpixwidth, double mzscale=1., double rtscale=1.):tbb::filter(parallel),
      mzpixelwidth_(mzpixwidth),rtpixelwidth_(rtpixwidth),mzscale_(mzscale), rtscale_(rtscale)
    {
    }

    void * operator()(void * queue)
    {
      ralab::LCMSImage * mp = static_cast<ralab::LCMSImage *>(queue);

      boost::timer time;
      time.restart();
      LOG(INFO) << "> start Image filtering";
      LCMSImageFilter imgf;
      imgf.filterMap( *mp , mzpixelwidth_ , rtpixelwidth_ , mzscale_, rtscale_);
      LOG(INFO) << "> Image Filtered in : " << time.elapsed() << " [s]";
      return mp;
    }
  };


  struct FeatureFind : tbb::filter{

    double minitensity_;

    FeatureFind(double minitensity):tbb::filter(parallel),minitensity_(minitensity)
    {}

    void * operator()(void * image)
    {
      ralab::LCMSImage * mp = static_cast<ralab::LCMSImage *>(image);

      boost::timer time;
      time.restart();
      LOG(INFO) << "> start Feature Find";
      ralab::FeatureFinder ff;
      ralab::FeaturesMap * map = new ralab::FeaturesMap();
      map->setMapDescription(mp->getMapDescription());

      ff.findFeature( mp->getMap(), minitensity_ );
      ff.extractFeatures(*map,mp->getMap());
      delete mp;
      LOG(INFO) << "> Features Found in " << time.elapsed() << " [s]";
      return map;
    }
  };

  struct WriteSQLFeatures : tbb::filter{
    ralab::FeaturesMapSQLWriterFacade facade_;
    WriteSQLFeatures(const std::string & outdir,const std::string & filestem):tbb::filter(serial_in_order),
      facade_( outdir , filestem )
    {}

    void * operator()(void * map)
    {
      ralab::FeaturesMap * fm = static_cast<ralab::FeaturesMap *>( map );

      boost::timer time;
      LOG(INFO) << "> start writing SQL" ;
      facade_.writeSQL2(*fm);
      LOG(INFO) << "> Features Written in " << time.elapsed() << " [s]";
      delete fm;

      return NULL;
    }
  };
}//end namespace ralab

int run_pipeline( int nthreads, ralab::Params & params, ralab::SwathInfoPtr sip )
{
  // Create the pipeline
  tbb::pipeline pipeline;
  // Create squaring stage and add it to the pipeline
  ralab::LCMSImageReaderFilter input(params, sip);
  // Create file-reading writing stage and add it to the pipeline
  pipeline.add_filter( input );
  ralab::MapFilter filter( params.mzpixelwidth , params.rtpixelwidth , params.mzscale , params.rtscale );
  pipeline.add_filter( filter );

  // Create file-writing stage and add it to the pipeline
  ralab::FeatureFind featurefind(params.minintensity);
  pipeline.add_filter( featurefind );

  ralab::WriteSQLFeatures writeFeatures( params.outdir , params.filestem_ );
  pipeline.add_filter( writeFeatures );

  // Run the pipeline
  tbb::tick_count t0 = tbb::tick_count::now();
  // Need more than one token in flight per thread to keep all threads
  // busy; 2-4 works

  pipeline.run( nthreads );
  tbb::tick_count t1 = tbb::tick_count::now();
  LOG(INFO) << (t1-t0).seconds();
  return 1;
}

//
int main(int argc, char *argv[])
{
  FLAGS_log_dir = ".";
  google::InitGoogleLogging(argv[0]);
  google::LogToStderr();

  // do some testing first.
  b_po::variables_map vmgeneral;
  parsecommandlineExtract(argc, argv, vmgeneral);

  ralab::Params pars;
  analysisParameters(pars,vmgeneral);

  boost::timer time;
  LOG(INFO) << "start reading properties" << std::endl;
  pwiz::msdata::MSDataPtr msdataptr = pwiz::msdata::MSDataPtr(new pwiz::msdata::MSDataFile(pars.infile));
  LOG(INFO)  << "time to open file " << time.elapsed() << std::endl;
  ralab::SwathPropertiesReader swathPropReader(msdataptr);

  LOG(INFO)  << "time required to scan through file :" << time.elapsed() << std::endl;



  LOG(INFO) << "ppm = " << pars.ppm ;
  pars.prepareOutputFile(false);
  ralab::FeaturesMapSQLWriterFacade facade(pars.outdir,pars.filestem_);
  facade.createDatabase();
  LOG(INFO) << "run pipeline with :" << pars.nrthreads ;
  run_pipeline(pars.nrthreads,pars,swathPropReader.getSwathInfo() );
  return 0;
}


