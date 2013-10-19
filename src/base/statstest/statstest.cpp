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
  class StatsTest : public ::testing::Test {
  protected:
    std::vector<double> m_data;
    std::vector<double> m_weight;

    // You can remove any or all of the following functions if its body
    // is empty.
    StatsTest() {
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
    virtual ~StatsTest() {
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

  // Tests that the Foo::Bar() method does Abc.
  TEST_F(StatsTest, computeStats) {
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
  TEST_F(StatsTest, moments) {
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

  TEST_F(StatsTest, weightedmoment){
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

   TEST_F(StatsTest, weightedSkewness){
     double res , epsilon(0.0001);
     double data[12] = {-2.75, -2.25, -1.75, -1.25, -0.75, -0.25,  0.25,  0.75,  1.25,  1.75,  2.25,  2.75};
     double weight[12] = { 6,  12,  42,  90, 145, 211, 187, 145, 92, 48, 16, 6};

     m_data.assign(data,data+12);
     m_weight.assign(weight, weight+12);

     res = ralab::stats::skewW(m_data.begin(),m_data.end(), m_weight.begin(), 0.0, 1.);
     res = ralab::stats::skewW(m_data.begin() ,m_data.end(), m_weight.begin() );

     /*exponential distribution with mean 1 and rate 1.*/
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

   TEST_F(StatsTest,weightedkurtosis ){
     double res , epsilon(0.0001);
     /*normal distribution with mean = 0 and sigma 1*/
     double data[12] = {-2.75, -2.25, -1.75, -1.25, -0.75, -0.25,  0.25,  0.75,  1.25,  1.75,  2.25,  2.75};
     double weight[12] = { 6,  12,  42,  90, 145, 211, 187, 145,  92,  48,  16,   6 };

     m_data.assign(data,data+12);
     m_weight.assign(weight, weight+12);
     res = ralab::stats::kurtW(m_data.begin(),m_data.end() , m_weight.begin() , 0.0 , 1. );
     res = ralab::stats::kurtW(m_data.begin(),m_data.end(), m_weight.begin() );

     /*cauchi distribution with mean 0 and rate 1.*/
     double datacauchy[13] = { -125,  -75,  -25,   25,   75,  125,  175,  225,  275,  325,  375,  425, 475 };
     double weightcauchy[13] = { 2 ,1 ,476 ,515 ,1 ,1 ,1 ,2 ,0 ,0 ,0 ,0 ,1 };

     m_data.assign(datacauchy,datacauchy+12);
     m_weight.assign(weightcauchy, weightcauchy+12);

     res = ralab::stats::kurtW(m_data.begin(), m_data.end(), m_weight.begin(), 1.0, 1.);
     ASSERT_TRUE( res > 0 ); // spitz zulaufende Verteilung
     res = ralab::stats::skewW(m_data.begin(),m_data.end() , m_weight.begin());
     ASSERT_TRUE( res > 0 ); // spitz zulaufende verteilung
   }

   TEST_F(StatsTest,testQuantileStats2){
     double epsilon(1e-10);
     std::vector<double> values ;
     ralab::base::base::seq(1.,20.,values);

     //Test Median
     double median = ralab::stats::median(values.begin(),values.end());
     ASSERT_NEAR( median , 10.5, epsilon);

     //Test Five numbers
     std::vector<double> quantiles;
     ralab::stats::fivenum(values,quantiles);
     std::vector<double> probs;
     probs.clear();
     probs.push_back(1.0);
     probs.push_back(5.5);
     probs.push_back(10.5);
     probs.push_back(15.5);
     probs.push_back(20);
     ASSERT_TRUE( std::equal(probs.begin(), probs.end(), quantiles.begin(), ralab::base::resample::DaCompFunctor<double>(0.01) ) );

     //Test Quantiles
     probs.clear();
     probs.push_back(0.0);
     probs.push_back(0.15);
     probs.push_back(0.25);
     probs.push_back(0.35);
     probs.push_back(0.5);
     probs.push_back(0.76);
     probs.push_back(0.9);
     probs.push_back(1);
     std::vector<double> ref;
     ref.push_back(1.00); // 1.00  3.85  5.75  7.65 10.50 15.44 18.10 20.00
     ref.push_back(3.5);
     ref.push_back(5.5);
     ref.push_back(7.5);
     ref.push_back(10.50);
     ref.push_back(15.5);
     ref.push_back(18.5);
     ref.push_back(20.00);

     ralab::stats::quantile(values, probs, quantiles);

     ASSERT_TRUE(std::equal(ref.begin(), ref.end(), quantiles.begin(), ralab::base::resample::DaCompFunctor<double>(0.01)) );
     //Test Range
     std::pair<double, double> range;
     ralab::base::base::Range(values, range );
     ASSERT_NEAR(range.first , 1 , 1e-10 );
     ASSERT_NEAR(range.second , 20 , 1e-10 );

   }

   TEST_F(StatsTest, quantileStats){
     double epsilon(0.01) ;
     std::vector<double> values ;
     ralab::base::stats::runif(10000,values, 0.,1.) ;
     //Test Median
     double median = ralab::stats::median(values.begin(),values.end());
     ASSERT_NEAR( median , 0.5, epsilon );
     //Test Quantiles
     std::vector<double> probs;
     probs.push_back(0.0);
     probs.push_back(0.15);
     probs.push_back(0.25);
     probs.push_back(0.35);
     probs.push_back(0.5);
     probs.push_back(0.76);
     probs.push_back(0.9);
     probs.push_back(1);
     std::vector<double> quantiles;
     ralab::stats::quantile(values, probs, quantiles);
     ASSERT_TRUE(std::equal(probs.begin(), probs.end(), quantiles.begin(), ralab::base::resample::DaCompFunctor<double>(epsilon) ));

     //Test Five numbers
     ralab::stats::fivenum(values,quantiles);
     probs.clear();
     probs.push_back(0);
     probs.push_back(0.25);
     probs.push_back(0.5);
     probs.push_back(0.75);
     probs.push_back(1);
     ASSERT_TRUE( std::equal(probs.begin(), probs.end(), quantiles.begin(), ralab::base::resample::DaCompFunctor<double>(epsilon) ) );
     //Test Range
     std::pair<double, double> range;
     ralab::base::base::Range(values, range );
     ASSERT_NEAR(range.first , 0 , 0.05 );
     ASSERT_NEAR(range.second , 1 , 0.05 );
   }
}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
