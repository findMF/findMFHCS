#include "gtest/gtest.h"
#include "base/stats/scale.h"
#include "base/base/base.h"
#include "base/resample/masscomparefunctors.h"

namespace {

  // The fixture for testing class Foo.
  class ScaleTest : public ::testing::Test {
  protected:
    // You can remove any or all of the following functions if its body
    // is empty.
    ScaleTest() {
      // You can do set-up work for each test here.
    }
    virtual ~ScaleTest() {
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

  TEST_F(ScaleTest,testRootMeanSquare)
  {
    double epsilon(5e-5);
    std::vector<double> x;
    ralab::base::base::seq(10,x);
    double res = ralab::base::stats::rootMeanSquare(x.begin(),x.end());
    ASSERT_NEAR(6.540472, res, epsilon );
  }

  TEST_F(ScaleTest,testScale)
  {
    double epsilon(5e-4);
    std::vector<double> x;
    ralab::base::base::seq(10,x);
    std::vector<double> tmp(x);
    std::pair<double,double> scaled;

    ralab::base::stats::scale(x.begin(),x.end(),scaled,false,false);
    bool isequal = std::equal(x.begin(),x.end(),tmp.begin(),ralab::base::resample::DaCompFunctor<double>(epsilon));
    ASSERT_TRUE(isequal);
    // test center=false , scale = true.
    //  c(scale(seq(1,10),F,T)) #(in R)
    double scaleFT[10] ={  0.1528942, 0.3057883, 0.4586825, 0.6115766, 0.7644708, 0.9173649, 1.0702591, 1.2231533, 1.3760474, 1.5289416};
    ralab::base::stats::scale(x.begin(),x.end(),scaled, false,true);
    isequal = std::equal(x.begin(), x.end(), scaleFT,ralab::base::resample::DaCompFunctor<double>(epsilon) );
    ASSERT_TRUE(isequal);
    ASSERT_NEAR( scaled.first, std::numeric_limits<double>::quiet_NaN() , epsilon);
    ASSERT_NEAR( scaled.second, 6.540472, epsilon);
    // test center=true , scale = false.
    //  c(scale(seq(1,10),T,F)) #(in R)
    x.assign(tmp.begin(),tmp.end());
    double scaleTF[10] ={-4.5, -3.5, -2.5, -1.5, -0.5,  0.5,  1.5,  2.5,  3.5,  4.5};
    ralab::base::stats::scale(x.begin(),x.end(),scaled, true,false);
    isequal = std::equal(x.begin(), x.end(), scaleTF,ralab::base::resample::DaCompFunctor<double>(epsilon) );
    ASSERT_TRUE(isequal);
    ASSERT_NEAR(scaled.first, 5.5, epsilon);
    ASSERT_NEAR(scaled.second, std::numeric_limits<double>::quiet_NaN(), epsilon);
    // test center=true , scale = false.
    //  c(scale(seq(1,10),T,T)) #(in R)
    x.assign(tmp.begin(),tmp.end());
    double scaleTT[10] ={-1.4863011, -1.1560120, -0.8257228, -0.4954337, -0.1651446,  0.1651446,  0.4954337,  0.8257228,  1.1560120,  1.4863011};
    ralab::base::stats::scale(x.begin(),x.end(),scaled, true,true);
    isequal = std::equal(x.begin(), x.end(), scaleTT,ralab::base::resample::DaCompFunctor<double>(epsilon) );
    ASSERT_TRUE(isequal);
    ASSERT_NEAR(scaled.first, 5.5, epsilon);
    ASSERT_NEAR(scaled.second, 3.027650, epsilon);

  }
}//namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


