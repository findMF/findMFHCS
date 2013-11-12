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

namespace{

  // The fixture for testing class Foo.
  class QuantilesTest : public ::testing::Test{
  protected:
    //std::vector<double> m_data;
    //std::vector<double> m_weight;

    // You can remove any or all of the following functions if its body
    // is empty.
    QuantilesTest() {
      // You can do set-up work for each test here.
    }
    // Objects declared here can be used by all tests in the test case for Foo.
  };

   TEST_F(QuantilesTest,testQuantileStats2){
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

   TEST_F(QuantilesTest, quantileStats){
     double epsilon(0.01) ;
     std::vector<double> values ;
     ralab::base::stats::runif(100000,values, 0.,1.) ;
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
