// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "findmf/datastruct/maplcmsdescription.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>
#include <iostream>


BOOST_AUTO_TEST_SUITE(MapLCMSDescriptionTest)

/*! \brief test filter function */
BOOST_AUTO_TEST_CASE( testMapDescription)
{
  //ralab::findmf::datastruct::MapLCMSDescription mlcd;
  std::cout << "testFeatureTable2" << std::endl;

  std::vector<double> rtproj,mzproj;

  mzproj.push_back(0.);
  mzproj.push_back(0.);
  mzproj.push_back(1.);
  mzproj.push_back(1.);
  //test
  rtproj.push_back(1.);
  rtproj.push_back(2.);
  rtproj.push_back(3.);
  rtproj.push_back(4.);

  ralab::findmf::datastruct::MapLCMSDescriptionPtr x =
      ralab::findmf::datastruct::MapLCMSDescriptionPtr(new ralab::findmf::datastruct::MapLCMSDescription());

  x->mslevel(1);
  x->extractionWindowMZ().first = 100.;
  x->extractionWindowMZ().second  = 125.;
  x->setMass( mzproj );
  x->setRT( rtproj );
  x->rtRange().first = 1000;
  x->rtRange().second = 1400;

  x->mzRange().first = 120.03;
  x->mzRange().second = 1444.03;
}
BOOST_AUTO_TEST_SUITE_END()

