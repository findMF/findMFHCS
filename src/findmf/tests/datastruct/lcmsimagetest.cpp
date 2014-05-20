#include "findmf/datastruct/lcmsimage.h"


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(LCMSImageTest)

/*! \brief test filter function */
BOOST_AUTO_TEST_CASE(testImage)
{
  ralab::findmf::datastruct::LCMSImage lcmsimage;
  lcmsimage.read("./submodule/findMFData/testS_437.tiff");
  std::vector<double> mz, rt;
  lcmsimage.getMZ(mz);
  rt.assign( lcmsimage.getRT().begin() , lcmsimage.getRT().end() );
  std::size_t x = lcmsimage.getBin(100.);
  std::size_t y = lcmsimage.getBin(1000.);
  std::vector<int> idx;
  lcmsimage.getBin(1000.,1003.,idx,rt);

  // TODO check why the image import does not work.
  std::cout << lcmsimage.getMZsize() << std::endl;
  std::cout << lcmsimage.getRTsize() << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()


