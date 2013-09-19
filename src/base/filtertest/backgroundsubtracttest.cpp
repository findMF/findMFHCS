// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "base/filter/scanfilter/backgroundsubtract.h"


#include <boost/timer.hpp>
#include "gtest/gtest.h"
#include "base/base/base.h"
#include "base/stats/uniform.h"
#include "base/stats/normal.h"
#include "base/base/cumsum.h"

namespace {

  // The fixture for testing class Foo.
  class BackgroundsubtractTest : public ::testing::Test {
  protected:
    std::vector<double> data , dcumsum , res , res2;

    // You can remove any or all of the following functions if its body
    // is empty.
    BackgroundsubtractTest() {
      // You can do set-up work for each test here.
    }
    virtual ~BackgroundsubtractTest() {
      // You can do clean-up work that doesn't throw exceptions here.
    }
    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp() {
      typedef double value_type;
      ralab::base::stats::rnorm( 100000 , data );
      ralab::base::cumsum(data.begin(),data.end() , dcumsum );
      //ralab::stats::base::seq_length(30.,100.,dcumsum.size(),mass);

      // Code here will be called immediately after the constructor (right
      // before each test).
    }

    virtual void TearDown() {
      // Code here will be called immediately after each test (right
      // before the destructor).
    }

    // Objects declared here can be used by all tests in the test case for Foo.
  };

  // Tests that the Foo::Bar() method does Abc.
  TEST_F(BackgroundsubtractTest, testseq) {
    res2.resize(dcumsum.size());
    boost::timer time;
    time.restart();
    ralab::base::filter::scanfilter::BackgroundSubtract<double> bs2(15);
    for(int i = 0; i < 100; ++i){
        bs2(dcumsum,res2);
      }
    std::cout << "openms : " << time.elapsed();
  }

  // Tests that Foo does Xyz.
  TEST_F(BackgroundsubtractTest, mean) {
  }

  TEST_F(BackgroundsubtractTest, geometricmean){
  }
}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
