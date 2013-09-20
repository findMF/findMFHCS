// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include <QApplication>
#include <iostream>
#include <boost/timer.hpp>

#include <glog/logging.h>

#include "algo/filter.h"
#include "fileio/spectramap.h"

#include "application/featurefinderapp.h"
#include "src/qtwidgets/TwoPanelImageWidget.h"

#include "parseargExtract.h"

#include "fileio/spectrastreader.h"
#include "fileio/helperfunctions.h"


int main(int argc, char *argv[])
{

  google::InitGoogleLogging(argv[0]);
  google::LogToStderr();

  // do some testing first.
  b_po::variables_map vmgeneral;
  parsecommandlineExtract(argc, argv, vmgeneral);

  AnalysisParameters ap;
  analysisParameters(ap,vmgeneral);

  boost::filesystem::path p1,p2;
  p1 = createOutputs(ap.infile,"");
  p2 = createOutputs(ap.infile,"fitered");

  if( !boost::filesystem::exists(ap.infile) )
    {
      std::cout << "could not find specified file :" ;
      std::cout << ap.infile << std::endl;
      return -1;
    }

  ralab::SpectraMap sm(ap.infile, ap.resolution);

  std::vector<std::size_t> keys;
  sm.getKeys(keys);
  boost::timer time;

  for(int i = 0 ; i < (keys.size()) ; ++i)
    {

      boost::filesystem::path px = createOutputs(p2.string(),boost::lexical_cast<std::string>(i));

      ralab::MapLCMSProcessorPtr mp = sm.getMap( keys[i] , ap.minmass );


      std::cout << "read : " << time.elapsed() << std::endl;
      mp->filterMap(ap.mzresolution, ap.rtresolution);
      mp->write_image( px.string() );

      std::cout << "read and filtered : " << time.elapsed() << std::endl;
      ralab::FeatureFinderApp ff;
      ff.findFeature( mp->getMap(), 10. );


      std::string file = p1.stem().string();
      file += boost::lexical_cast<std::string>(i);
      ff.writeFeatures(ap.outdir , file );
      ff.writeFeatures2SQL(ap.outdir , file);
      std::cout << "written features : " << time.elapsed() << std::endl;
    }
  return 0;
}


