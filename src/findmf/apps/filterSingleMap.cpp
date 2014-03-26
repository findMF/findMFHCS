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

#include "findmf/algo/vigra/lcmsimagefilter.h"
#include "findmf/fileiopwiz/lcmsimagereader.h"

//#include "findmf/application/featuresmapprinter.h"
#include "parseargsImageExport.h"
#include "findmf/fileio/sqlite2/featuresmapsqlwriterfacade.h"
#include "findmf/algo/vigra/featurefinder.h"
#include "findmf/algo/vigra/ComputeFeatureStatistics.h"

#include "findmf/apps/toolparameters.h"

namespace ralab
{
  /// process single swath or ms map.
  struct SwathProcessor{
    std::vector< std::size_t > keys;
    ralab::findmf::datastruct::MSFileInfoPtr sip_ ;
    ralab::findmf::apps::Params anap_ ;
    int mapid_;

    SwathProcessor( const ralab::findmf::apps::Params & ap, // the analysis parameter
                    ralab::findmf::datastruct::MSFileInfoPtr sip, // swath info pointer
                    int mapid
                    ) :sip_(sip), anap_(ap),mapid_(mapid)
    {
      anap_.prepareOutputFile(true);
    }

    void operator()(void) {

      sip_->getKeys(keys);
      ralab::findmf::datastruct::LCMSImage mp;
      try{
        pwiz::msdata::MSDataPtr msdataptr = pwiz::msdata::MSDataPtr(new pwiz::msdata::MSDataFile(anap_.infile));
        ralab::findmf::LCMSImageReader tmp ( msdataptr , sip_ , anap_.ppm );
        std::cerr << "processing map : " << mapid_ << " with key : " << keys[mapid_]<< std::endl ;
        tmp.getMap( keys[ mapid_ ] , anap_.minmass,anap_.maxmass, mp );
      }catch(std::exception &e){
        std::cerr << "reading failed" << e.what() << std::endl;
        return;
      }

      try{
        ralab::findmf::LCMSImageFilter imgf;
        imgf.filterMap(mp.getImageMap().getMap(), anap_.mzpixelwidth , anap_.rtpixelwidth, anap_.mzscale , anap_.rtscale );
        mp.getImageMap().updateImageRange();
        mp.getImageMap().write( anap_.outdir, anap_.filestem_ );

        pwiz::msdata::MSDataPtr msdataptr = pwiz::msdata::MSDataPtr(new pwiz::msdata::MSDataFile(anap_.infile));
        ralab::findmf::LCMSImageReader tmp ( msdataptr , sip_ , anap_.ppm );

        ///TODO test.
        boost::filesystem::path x = anap_.outdir_ / anap_.filestem_ ;
        tmp.write(x.string(),mp);

      }catch(std::exception &e){
        std::cerr << "filtering failed " << e.what() << std::endl;
        return;
      }


    }
  };
}//end namespace ralab

///this for tbb code
template <typename T> struct invoker {
  void operator()(T& it) const {it();}
};

//test
int main(int argc, char *argv[])
{

  // TODO maybe should go in a single class.
  b_po::variables_map vmgeneral;
  parsecommandlineExtract(argc, argv, vmgeneral);
  ralab::findmf::apps::Params pars;
  analysisParameters(pars,vmgeneral);

  std::cerr << "ppm is:" << pars.ppm << std::endl;


  ///ralab::findmf::FeaturesMapSQLWriterFacade facade(pars.outdir,pars.filestem_);
  ///facade.createDatabase();

  //////
  boost::timer time;

  ralab::findmf::SwathPropertiesReader swathPropReader(pars.infile);
  std::vector<std::size_t> keys;
  swathPropReader.getSwathInfo()->getKeys(keys);

  //LOG(INFO)  << "time required to scan through file :" << time.elapsed() << std::endl;

  tbb::task_scheduler_init init(pars.nrthreads);
  std::vector<ralab::SwathProcessor> tasks;
  for(std::size_t i = 0 ; i < keys.size() ; ++i){
      tasks.push_back(ralab::SwathProcessor( pars , swathPropReader.getSwathInfo(),i));
    }
  tbb::parallel_for_each(tasks.begin(),tasks.end(),invoker<ralab::SwathProcessor>());
  return 0;
}


