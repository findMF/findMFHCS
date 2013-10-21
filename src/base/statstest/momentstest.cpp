// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#include "gtest/gtest.h"
#include "base/stats/quantiles.h"

#include "base/stats/momentsW.h"
#include "base/stats/moments.h"

#include "base/base/base.h"
#include "base/resample/masscomparefunctors.h"
#include "base/stats/uniform.h"

namespace {

  // The fixture for testing class Foo.
  class MomentsTest : public ::testing::Test {
  protected:
    std::vector<double> m_data;
    std::vector<double> m_weight;

    // You can remove any or all of the following functions if its body
    // is empty.
    MomentsTest() {
      m_data.push_back(2);
      m_data.push_back(4);
      m_data.push_back(8);
      m_data.push_back(10);
      m_data.push_back(99);
      m_data.push_back(1);

      m_weight.push_back(2);
      m_weight.push_back(3);
      m_weight.push_back(4);
      m_weight.push_back(5);
      m_weight.push_back(6);
      m_weight.push_back(1);
      // You can do set-up work for each test here.
    }
  };

  // Tests that the Foo::Bar() method does Abc.
  TEST_F(MomentsTest, computeStats) {
    double epsilon = 0.0001;
    double sum, mean, var, dev, skew, kurt;
    ralab::stats::computeStats(m_data.begin( ), m_data.end( ),
                              sum, mean, var, dev, skew, kurt);
    ASSERT_NEAR(sum  ,124.0, epsilon);
    ASSERT_NEAR(mean ,20.6667, epsilon);
    ASSERT_NEAR(var  ,1237.22222, epsilon);
    ASSERT_NEAR(dev  ,35.1742, epsilon );
    ASSERT_NEAR(skew ,1.75664, epsilon) ;
    ASSERT_NEAR(kurt ,1.14171, epsilon );
  }

  // Tests that Foo does Xyz.
  TEST_F(MomentsTest, moments) {
    double res, epsilon = 0.0001;
    res = ralab::stats::var( m_data.begin( ), m_data.end( ));
    ASSERT_NEAR(res, 1237.222222, epsilon);
    res = ralab::stats::sd( m_data.begin( ) , m_data.end( ));
    ASSERT_NEAR(res, 35.1742, epsilon);
    res = ralab::stats::skew( m_data.begin( ), m_data.end( ));
    ASSERT_NEAR(res, 1.75664, epsilon);
    res = ralab::stats::kurtosis( m_data.begin( ) , m_data.end( ));
    ASSERT_NEAR(res, 1.14171, epsilon);
    //test the unbiased versions.
    res = ralab::stats::varUnbiased( m_data.begin( ), m_data.end( ));
    ASSERT_NEAR(res, 1484.66666, epsilon);
    res = ralab::stats::sdUnbiased( m_data.begin( ), m_data.end( ));
    ASSERT_NEAR(res, 38.53137, epsilon);
  }


}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
