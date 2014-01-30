// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include <iostream>
#include <boost/timer.hpp>

#include <tbb/task_scheduler_init.h>
#include <tbb/concurrent_queue.h>

#include "findmf/tbbfilters/lcmsimagereader.h"
#include "findmf/tbbfilters/writeImage.h"
#include "findmf/tbbfilters/mapfilter.h"

#include "findmf/apps/toolparameters.h"
#include "parseargExtract.h"

/// build the pipeline
int run_pipeline( int nthreads, ralab::findmf::apps::Params & params, ralab::findmf::datastruct::MSFileInfoPtr sip )
{
  // Create the pipeline
  tbb::pipeline pipeline;
  // Create squaring stage and add it to the pipeline
  params.prepareOutputFile(false);
  ralab::findmf::LCMSImageReaderFilter::Params lcmspar;
  lcmspar.infile_ = params.infile;
  lcmspar.maxmass_ = params.maxmass;
  lcmspar.minmass_ = params.minmass;
  lcmspar.ppm_ = params.ppm;
  lcmspar.rt2sum_ = params.rt2sum_;
  // Create file-reading writing stage and add it to the pipeline
  ralab::findmf::LCMSImageReaderFilter input( lcmspar, sip);
  pipeline.add_filter( input );

  if(params.dofilter){
    ralab::findmf::MapFilter imagefilter(params.mzpixelwidth,
                                         params.rtpixelwidth,
                                         params.mzscale,
                                         params.rtscale);
    pipeline.add_filter(imagefilter);
  }

  // Create image writer and addit to the pipeline
  ralab::findmf::WriteImage wi(params.outdir_);
  pipeline.add_filter(wi);
  pipeline.run( nthreads );

  return 1;
}

//
int main(int argc, char *argv[])
{
  // do some testing first.
  b_po::variables_map vmgeneral;
  parsecommandlineExtract(argc, argv, vmgeneral);

  ralab::findmf::apps::Params pars;
  analysisParameters(pars,vmgeneral);
  pars.prepareOutputFile(false);


  // open file.
  //pwiz::msdata::MSDataPtr msdataptr;
  ralab::findmf::datastruct::MSFileInfoPtr sip;
  try{
    ralab::findmf::SwathPropertiesReader swathPropReader(pars.infile);
    sip = swathPropReader.getSwathInfo();
  } catch(std::exception & e){
    std::cerr << "infile : " << pars.infile << std::endl;
    std::cerr  << "can't open file: " << e.what() << std::endl;
    return 0;
  }

  std::cerr << "run pipeline with :" << pars.nrthreads ;
  run_pipeline(pars.nrthreads, pars, sip );
  return 0;
}


