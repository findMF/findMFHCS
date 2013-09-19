// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "base/filter/filter.h"


#include <boost/timer.hpp>
#include "gtest/gtest.h"
#include "base/base/base.h"
#include "base/stats/uniform.h"
#include "base/stats/normal.h"
#include "base/base/cumsum.h"
#include "base/resample/masscomparefunctors.h"

namespace {

  // The fixture for testing class Foo.
  class FilterUtilsTest : public ::testing::Test {
  protected:
    std::vector<double> data , dcumsum , res , res2;

    // You can remove any or all of the following functions if its body
    // is empty.
    FilterUtilsTest() {
      // You can do set-up work for each test here.
    }
    virtual ~FilterUtilsTest() {
      // You can do clean-up work that doesn't throw exceptions here.
    }
    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp() {
    }

    virtual void TearDown() {
      // Code here will be called immediately after each test (right
      // before the destructor).
    }

    // Objects declared here can be used by all tests in the test case for Foo.
  };



  /*! \brief test filter function */
  TEST_F( FilterUtilsTest,filterTest)
  {
    double epsilon = 0.000001;
    epsilon;
    std::vector<double> data;
    ralab::base::base::seq(-500., 500., .1, data);
    std::vector<double> y;
    std::transform(data.begin(), data.end(), std::back_inserter(y), static_cast<double(*)(double)>(sin) );

    std::vector<double> filt3(21,1./21.); // mean filter
    double sumfilt =   std::accumulate(filt3.begin(), filt3.end(), 0.0 );
    ASSERT_NEAR(sumfilt, 1.0,epsilon );

    std::vector<double> result;
    ralab::base::filter::filter(
          y,
          filt3,
          result
          );

    ralab::base::filter::filter(
          y,
          filt3,
          result,
          true
          );

    result.resize(y.size());

    ralab::base::filter::filter_sequence(
          y.begin(),
          y.end(),
          filt3.begin(),
          filt3.size(),
          result.begin(),
          true
          );

  }

  /*! \brief Evaluate data extension */
  TEST_F( FilterUtilsTest,testExtendData)
  {
    std::vector<int> tmp, res;
    ralab::base::base::seq(5,tmp);
    std::vector<int>::iterator it = ralab::base::filter::utilities::prepareData(tmp.begin(),tmp.end(), 5, res);
    res.resize(std::distance(res.begin(),it));
    double ref[] = { 4, 5, 1, 2, 3, 4 ,5, 1, 2};

    /*std::copy(res.begin(),res.end(),std::ostream_iterator<int>(std::cout," "));
                              std::cout << std::endl;
                              */
    bool iseq = std::equal(res.begin(),res.end(),ref);
    ASSERT_TRUE(iseq);

    it = ralab::base::filter::utilities::prepareData(tmp.begin(),tmp.end(), 5, res, true);
    res.resize(std::distance(res.begin(),it));

    double ref2[] = {2, 1, 1, 2, 3, 4, 5, 5, 4};
    iseq = std::equal(res.begin(),res.end(),ref2);
    ASSERT_TRUE(iseq);
  }

}//end namespace


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}





