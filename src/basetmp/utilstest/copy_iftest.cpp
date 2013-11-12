// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "gtest/gtest.h"
#include "base/utils/copyif.h"

#include "base/resample/masscomparefunctors.h"
#include <boost/bind.hpp>


namespace {

  // The fixture for testing class Foo.
  class Copy_iftest : public ::testing::Test {
  protected:
    // You can remove any or all of the following functions if its body
    // is empty.
    Copy_iftest() {
      // You can do set-up work for each test here.
    }
    virtual ~Copy_iftest() {
      // You can do clean-up work that doesn't throw exceptions here.
    }
    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp() {
      // Code here will be called immediately after the constructor (right
      // before each test).
    }

    virtual void TearDown() {
      // Code here will be called immediately after each test (right
      // before the destructor).
    }

    // Objects declared here can be used by all tests in the test case for Foo.
  };

  TEST_F(Copy_iftest,copyif)
  {
    double x[] = {1,3,5,1,3,5,1,3,5,1,3,5};
    double y[] = {2,4,6,2,4,6,2,4,6,2,4,6};

    std::vector<double> res;

    ralab::base::utils::copy_if(x,x+sizeof(x)/sizeof(double),std::back_inserter(res),boost::bind(std::less<double>(), _1, 2 ) );
    ASSERT_EQ(res.size(),4);
    res.clear();
    ralab::base::utils::copy_if_not(x,x+sizeof(x)/sizeof(double),std::back_inserter(res),boost::bind(std::less<double>(), _1, 2 ) );
    ASSERT_EQ(res.size(),8);
    res.clear();
  }

  //tmp
  TEST_F(Copy_iftest,copyif2)
  {
    double x[] = {1,3,5,1,3,5,1,3,5,1,3,5};
    double y[] = {2,4,6,2,4,6,2,4,6,2,4,6};

    std::vector<double> res;
    ralab::base::utils::copy_if(x,x+sizeof(x)/sizeof(double),y,
                                std::back_inserter(res),
                                boost::bind(std::less<double>(), _1, 2 ) );
    ASSERT_EQ(res.size(),4);
    ASSERT_EQ(res[0],2);
    res.clear();
    ralab::base::utils::copy_if_not(x,x+sizeof(x)/sizeof(double),y,
                                    std::back_inserter(res),boost::bind(std::less<double>(), _1, 2 ) );
    ASSERT_EQ(res.size(),8);
    ASSERT_EQ(res[0],4);
    res.clear();
  }
}//namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


