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

#include "findmf/application/lcmsimagefilter.h"
#include "findmf/fileiopwiz/lcmsimagereader.h"

#include "findmf/application/featuresmapprinter.h"
#include "parseargExtract.h"
#include "findmf/fileio/sqlite2/featuresmapsqlwriterfacade.h"
#include "findmf/algo/vigra/featurefinder.h"
#include "findmf/apps/toolparameters.h"

namespace ralab
{
  //process single swath or ms map.
  struct SwathProcessor{
    std::vector< std::size_t > keys;
    ralab::findmf::datastruct::SwathInfoPtr sip_ ;
    ralab::findmf::apps::Params anap_ ;
    ralab::findmf::FeaturesMapSQLWriterFacade sqlwriter_;

    SwathProcessor( const ralab::findmf::apps::Params & ap, // the analysis parameter
                    ralab::findmf::datastruct::SwathInfoPtr sip, // swath info pointer
                    ralab::findmf::FeaturesMapSQLWriterFacade &sfs
                    ) :sip_(sip), anap_(ap),sqlwriter_(sfs)
    {
      anap_.prepareOutputFile();
    }

    //test ...
    void operator()(void) {
      sip_->getKeys(keys);
      ralab::findmf::datastruct::LCMSImage mp;
      try{
        pwiz::msdata::MSDataPtr msdataptr = pwiz::msdata::MSDataPtr(new pwiz::msdata::MSDataFile(anap_.infile));
        ralab::findmf::LCMSImageReader tmp ( msdataptr , sip_ , anap_.ppm );
        std::cerr << "processing map : " << anap_.i_ << " with key : " << keys[anap_.i_]<< std::endl ;
        tmp.getMap( keys[ anap_.i_ ] , anap_.minmass,anap_.maxmass, mp );
      }catch(std::exception &e){
        std::cerr << "reading failed" << e.what() << std::endl;
        return;
      }

      boost::timer time;
      try{
        ralab::findmf::LCMSImageFilter imgf;
        imgf.filterMap(mp, anap_.mzpixelwidth , anap_.rtpixelwidth, anap_.mzscale , anap_.rtscale );
        boost::filesystem::path x = anap_.outdir_ / anap_.filestem_ ;
        mp.write_image( x.string() );
        //TODO add option to switch wring tiff on and off.
        mp.write( x.string());
      }catch(std::exception &e){
        std::cerr << "filtering failed " << e.what() << std::endl;
        return;
      }

      ralab::findmf::datastruct::FeaturesMap map;
      map.setMapDescription( mp.getMapDescription() );

      try{
        time.restart();
        ralab::findmf::FeatureFinder ff;
        ff.findFeature( mp.getMap(), 10. );
        ff.extractFeatures(map,mp.getMap());

        //ff.writeFeatures( anap_.outdir_.string() , anap_.filestem_ );
        //ralab::FeaturesMapPrinter fp;
        //fp.writeFeatures( anap_.outdir_.string() , anap_.filestem_ , map);

      }catch(std::exception & e){
        std::cerr << "feature finding failed " << e.what() << std::endl;
        return;
      }

      try{
        time.restart();
        sqlwriter_.writeSQL2( map );
        //LOG(INFO) << "written features : " << time.elapsed();
      }catch(std::exception &e)
      {
        std::cerr << "error wrting sql" << e.what() << std::endl;
        return;
      }

      //LOG(INFO) << "map " << anap_.i_ << " processed." ;
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
  pars.i_ = 0;

  ///// prepare single feature storage for all lscms's
  pars.prepareOutputFile();
  ralab::findmf::FeaturesMapSQLWriterFacade facade(pars.outdir,pars.filestem_);
  facade.createDatabase();

  //////
  boost::timer time;
  //LOG(INFO) << "start reading properties" << std::endl;
  pwiz::msdata::MSDataPtr msdataptr = pwiz::msdata::MSDataPtr(new pwiz::msdata::MSDataFile(pars.infile));
  //LOG(INFO)  << "time to open file " << time.elapsed() << std::endl;

  ralab::findmf::SwathPropertiesReader swathPropReader(msdataptr);
  std::vector<std::size_t> keys;
  swathPropReader.getSwathInfo()->getKeys(keys);

  //LOG(INFO)  << "time required to scan through file :" << time.elapsed() << std::endl;

  tbb::task_scheduler_init init(pars.nrthreads);
  std::vector<ralab::SwathProcessor> tasks;
  for(std::size_t i = 0 ; i < keys.size() ; ++i){
      pars.i_ = i;
      tasks.push_back(ralab::SwathProcessor( pars , swathPropReader.getSwathInfo(),facade));
    }
  tbb::parallel_for_each(tasks.begin(),tasks.end(),invoker<ralab::SwathProcessor>());
  return 0;
}


