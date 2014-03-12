// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include <iostream>
#include <boost/timer.hpp>
#include <boost/thread.hpp>

#include <tbb/task_scheduler_init.h>
#include <tbb/pipeline.h>
#include <tbb/concurrent_queue.h>

#include "findmf/tbbfilters/lcmsimagereader.h"
#include "findmf/tbbfilters/mapfilter.h"
#include "findmf/tbbfilters/featurefind.h"
#include "findmf/tbbfilters/writesqlfeatures.h"


#include "findmf/fileio/helperfunctions.h"
#include "parseargExtract.h"

#include "findmf/apps/toolparameters.h"

/// run pipeline
int run_pipeline( int nthreads, ralab::findmf::apps::Params & params, ralab::findmf::datastruct::MSFileInfoPtr sip )
{
  // Create the pipeline
  tbb::pipeline pipeline;
  // Create squaring stage and add it to the pipeline
  params.prepareOutputFile();
  ralab::findmf::LCMSImageReaderFilter::Params lcmsparam;

  lcmsparam.infile_ = params.infile;
  lcmsparam.maxmass_ = params.maxmass;
  lcmsparam.minmass_ = params.minmass;
  lcmsparam.ppm_ = params.ppm;
  lcmsparam.rt2sum_ = params.rt2sum_;

  // Create file-reading writing stage and add it to the pipeline
  ralab::findmf::LCMSImageReaderFilter input(lcmsparam, sip);
  pipeline.add_filter( input );

  // Create filter node and add to pipeline
  ralab::findmf::MapFilter filter( params.mzpixelwidth , params.rtpixelwidth , params.mzscale , params.rtscale );
  pipeline.add_filter( filter );

  // Create file-writing stage and add it to the pipeline
  ralab::findmf::FeatureFind featurefind(params.minintensity);
  pipeline.add_filter( featurefind );

  // Create SQL writer and add to pipeline
  ralab::findmf::WriteSQLFeatures writeFeatures( params.outdir , params.filestem_ , params.writeprojections_ );
  pipeline.add_filter( writeFeatures );


  pipeline.run( nthreads );
   return 1;
}


int main(int argc, char *argv[])
{

  // do some testing first.
  b_po::variables_map vmgeneral;
  parsecommandlineExtract(argc, argv, vmgeneral);

  ralab::findmf::apps::Params pars;
  analysisParameters(pars,vmgeneral);

  ralab::findmf::datastruct::MSFileInfoPtr fileinfoptr;
  try{
    ralab::findmf::SwathPropertiesReader swathPropReader(pars.infile);
    fileinfoptr = swathPropReader.getSwathInfo();
  } catch(std::exception & e){
    std::cerr << "infile : " << pars.infile << std::endl;
    std::cerr  << "can't open file: " << e.what() << std::endl;
    return 0;
  }

  //LOG(INFO)  << "time required to scan through file :" << time.elapsed() << std::endl;
  pars.prepareOutputFile(false);
  ralab::findmf::FeaturesMapSQLWriterFacade facade(pars.outdir,pars.filestem_);
  facade.createDatabase();
  std::cerr << "run pipeline with :" << pars.nrthreads ;
  run_pipeline(pars.nrthreads,pars, fileinfoptr);
  return 0;
}


