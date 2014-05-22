#include "findmf/datastruct/lcmsimage.h"


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Lcmsimage
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(LCMSImageTest)

BOOST_AUTO_TEST_CASE(testPlainRead)
{
  boost::filesystem::path p1("./submodule/findMFData/testS_437.tiff");
  vigra::ImageImportInfo info(p1.string().c_str());
  //vigra::MultiArray<2, float> image(info.width(), info.height());
  vigra::MultiArray<2, float> image(info.width(), info.height());

  if(info.isGrayscale())
  {
    //map_.reshape(Map::difference_type(info.width(), info.height()));
    vigra::importImage(info, image);

  }
}


/*! \brief test filter function */
BOOST_AUTO_TEST_CASE(testImage)
{
  ralab::findmf::datastruct::LCMSImage lcmsimage;
  lcmsimage.read("./submodule/findMFData/testS_437.tiff");
  uint32_t mzsiz = lcmsimage.getMZsize();
  uint32_t rtsiz = lcmsimage.getRTsize();
  BOOST_CHECK_EQUAL(mzsiz, 13296);
  BOOST_CHECK_EQUAL(rtsiz,584);

  std::vector<double> mz, rt;

  lcmsimage.getMZ(mz);
  BOOST_CHECK_EQUAL(mz.size(),13296);
  rt.assign( lcmsimage.getRT().begin() , lcmsimage.getRT().end() );
  BOOST_CHECK_EQUAL(rt.size(),584);

  // test getting the bin of mass range
  std::size_t x = lcmsimage.getBin(100.);
  BOOST_CHECK_EQUAL(x,0);

  std::size_t y = lcmsimage.getBin(1000.);
  BOOST_CHECK_EQUAL(y,13296);

  std::vector<int> idx;
  lcmsimage.getBin(1000.,1003.,idx,rt);



}

BOOST_AUTO_TEST_SUITE_END()


