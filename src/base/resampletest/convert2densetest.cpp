// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "base/resample/convert2dense.h"

#include <gtest/gtest.h>
#include "base/stats/uniform.h"
#include "base/base/base.h"
#include "base/base/interpolate.h"
#include "base/base/cumsum.h"

namespace{

  class Convert2DenseTest: public ::testing::Test{
  protected:
    Convert2DenseTest(){}
  };

  TEST_F(Convert2DenseTest,testConvert)
  {
    std::pair<double, double> tmp;
    tmp.first = 1000.;
    tmp.second = 1010;
    double ppm = 100;
    ralab::base::resample::Convert2Dense c2d;
    c2d.defBreak(tmp,ppm);
    std::vector<double> mids;
    c2d.getMids(mids);
    c2d.am_ = 0.1;

    double mz[] = {1001. , 1001.5 , 1001.8 , 1004. , 1005., 1008. , 1009. , 1009.3};
    size_t smz = sizeof(mz)/sizeof(double);
    std::vector<double> intensity( smz, 10.), gg;
    double all = std::accumulate(intensity.begin(),intensity.end(),0.);

    c2d.convert2dense(mz,mz+smz,intensity.begin(),gg);
    std::cout << gg.size() << std::endl;

    double bla = std::accumulate(gg.begin(),gg.end(),0.);
    ASSERT_NEAR(bla, all - 10., 1e-10);
    // minus 10 because last intensity isn't added.
  }

}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
