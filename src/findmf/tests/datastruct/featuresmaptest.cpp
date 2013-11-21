#ifndef FEATURESMAPTEST_H
#define FEATURESMAPTEST_H
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>

#include "findmf/datastruct/featuresmap.h"

BOOST_AUTO_TEST_SUITE(FeatureMapTest)

BOOST_AUTO_TEST_CASE( testdiff)
{
  BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()

#endif // FEATURESMAPTEST_H
