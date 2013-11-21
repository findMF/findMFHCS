// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>

#include "findmf/algo/projectionstats.h"

#ifndef FEATURESMAPTEST_H
#define FEATURESMAPTEST_H

#include "findmf/algo/projectionstats.h"

// The fixture for testing class Foo.

BOOST_AUTO_TEST_SUITE(ProjectionStatsTest)

BOOST_AUTO_TEST_CASE( testconst ){
  std::vector<float> rtproj;
  std::vector<float> mzproj;
  float rt[] = {100,120,190,220,1000,200,100};
  float mz[] = {200,220,390,420,1000,300,200};

  rtproj.assign(rt,rt+sizeof(rt)/sizeof(float));
  mzproj.assign(mz,mz+sizeof(mz)/sizeof(float));
  ralab::findmf::projstats rtp , mzp;

  for(int i = 0 ; i < 10; ++i){
    ralab::findmf::utilities::computeStats(rtproj,100,rtp);
    ralab::findmf::utilities::Pick p;
    float peakrt =  p.pick( rtproj , rtp );
    BOOST_CHECK_CLOSE(peakrt,103.975906,1e-4);
    ralab::findmf::utilities::computeStats(mzproj,100,mzp);
    float peakmz =  p.pick( mzproj , mzp );
    BOOST_CHECK_CLOSE(peakmz, 103.835617, 1e-4 );

  }
}
BOOST_AUTO_TEST_SUITE_END()

#endif // FEATURESMAPTEST_H
