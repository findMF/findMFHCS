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
  class MomentsWTest : public ::testing::Test {
  protected:
    std::vector<double> m_data;
    std::vector<double> m_weight;

    // You can remove any or all of the following functions if its body
    // is empty.
    MomentsWTest() {
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

    // Objects declared here can be used by all tests in the test case for Foo.
  };
  TEST_F(MomentsWTest, weightedmoment){
    double res , epsilon(0.0001);
    res = ralab::stats::meanW(m_data.begin(),m_data.end() , m_weight.begin());
    ASSERT_NEAR(res,33.0,epsilon);
    res = ralab::stats::varW(m_data.begin(),m_data.end() , m_weight.begin() , 33.0 );
    ASSERT_NEAR(res,1750.0,epsilon);
    res = ralab::stats::varW(m_data.begin() , m_data.end(), m_weight.begin());
    ASSERT_NEAR(res,1750.0,epsilon);

    res = ralab::stats::sdW(m_data.begin(),m_data.end() , m_weight.begin() , 33.0 );
    ASSERT_NEAR(res,41.833,epsilon);
    res = ralab::stats::sdW(m_data.begin() , m_data.end(), m_weight.begin() );
    ASSERT_NEAR(res,41.833,epsilon);
    res = ralab::stats::varWUnbiased(m_data.begin(),m_data.end(), m_weight.begin());
    ASSERT_NEAR(res,2205,epsilon);
    res = ralab::stats::sdWUnbiased(m_data.begin(),m_data.end(), m_weight.begin());
    ASSERT_NEAR(res,46.95743,epsilon);
  }

   TEST_F(MomentsWTest, weightedSkewness){
     double res , epsilon(0.0001);
     double data[12] = {-2.75, -2.25, -1.75, -1.25, -0.75, -0.25,  0.25,  0.75,  1.25,  1.75,  2.25,  2.75};
     double weight[12] = { 6,  12,  42,  90, 145, 211, 187, 145, 92, 48, 16, 6};

     m_data.assign(data,data+12);
     m_weight.assign(weight, weight+12);

     res = ralab::stats::skewW(m_data.begin(),m_data.end(), m_weight.begin(), 0.0, 1.);
     res = ralab::stats::skewW(m_data.begin() ,m_data.end(), m_weight.begin() );

     ///exponential distribution with mean 1 and rate 1.
     double dataexp [13] = {0.25, 0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25, 4.75, 5.25, 5.75, 6.25 };
     double weightexp[13] = {412, 231 ,138 ,99 ,50 , 26, 19 ,14 , 3, 4, 1, 0, 3};

     m_data.assign(dataexp,dataexp+12);
     m_weight.assign(weightexp, weightexp+12);

     res = ralab::stats::skewW(m_data.begin(), m_data.end(), m_weight.begin(), 1., 1.);
     ASSERT_TRUE( res > 0 );
     res = ralab::stats::skewW(m_data.begin(), m_data.end(), m_weight.begin());
     ASSERT_TRUE( res > 0 );


     std::reverse(m_weight.begin(), m_weight.end());
     res = ralab::stats::skewW(m_data.begin(), m_data.end(), m_weight.begin(), 5., 1.);
     ASSERT_TRUE( res < 0 );
     res = ralab::stats::skewW(m_data.begin(), m_data.end(), m_weight.begin());
     ASSERT_TRUE( res < 0 );
   }

   TEST_F(MomentsWTest,weightedkurtosis ){
     double res , epsilon(0.0001);
     ///normal distribution with mean = 0 and sigma 1
     double data[12] = {-2.75, -2.25, -1.75, -1.25, -0.75, -0.25,  0.25,  0.75,  1.25,  1.75,  2.25,  2.75};
     double weight[12] = { 6,  12,  42,  90, 145, 211, 187, 145,  92,  48,  16,   6 };

     m_data.assign(data,data+12);
     m_weight.assign(weight, weight+12);
     res = ralab::stats::kurtW(m_data.begin(),m_data.end() , m_weight.begin() , 0.0 , 1. );
     res = ralab::stats::kurtW(m_data.begin(),m_data.end(), m_weight.begin() );

     ///cauchi distribution with mean 0 and rate 1.
     double datacauchy[13] = { -125,  -75,  -25,   25,   75,  125,  175,  225,  275,  325,  375,  425, 475 };
     double weightcauchy[13] = { 2 ,1 ,476 ,515 ,1 ,1 ,1 ,2 ,0 ,0 ,0 ,0 ,1 };

     m_data.assign(datacauchy,datacauchy+12);
     m_weight.assign(weightcauchy, weightcauchy+12);

     res = ralab::stats::kurtW(m_data.begin(), m_data.end(), m_weight.begin(), 1.0, 1.);
     ASSERT_TRUE( res > 0 );
     res = ralab::stats::skewW(m_data.begin(),m_data.end() , m_weight.begin());
     ASSERT_TRUE( res > 0 );
   }

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
