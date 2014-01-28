#include "findmf/datastruct/Map2D.h"


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>




BOOST_AUTO_TEST_SUITE(Map2DTest)

/*! \brief test filter function */
BOOST_AUTO_TEST_CASE( testMap)
{
  typedef ralab::findmf::datastruct::Map2D<float> MMap;
  MMap::Map tmp;
  MMap map;

  map.resize(100,100);
  map.put(3,3,42);
  BOOST_CHECK(42 == map.get(3,3));

  map.setMap(tmp);
  tmp = map.getMap();
  map.read("test.txt");
}

BOOST_AUTO_TEST_SUITE_END()

