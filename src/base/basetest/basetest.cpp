// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "gtest/gtest.h"
#include "base/base/base.h"

namespace {

  // The fixture for testing class Foo.
  class BaseTest : public ::testing::Test {
  protected:
    // You can remove any or all of the following functions if its body
    // is empty.
    BaseTest() {
      // You can do set-up work for each test here.
    }
    virtual ~BaseTest() {
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
  TEST_F(BaseTest, testseq) {
    std::vector<double> res;
    ralab::base::base::seq(1.,10.,0.5,res);
    ralab::base::base::seq(10.,2.,res);
    ralab::base::base::seq(2.,10.,res);
    ralab::base::base::seq(10., 2. , -0.5 , res);

    std::vector<int32_t> res2;
    ralab::base::base::seq(10,2,res2);
    ralab::base::base::seq(10,res);
    ralab::base::base::seq(res2,res);

    std::vector<unsigned int> resunsigned;
    ralab::base::base::seq(1u,10u,1u,resunsigned);
    ralab::base::base::seq(1u,10u,resunsigned);

    std::vector<double> resdouble;
    ralab::base::base::seq_length(100. , 1300.,18467,resdouble);
    std::cout << resdouble[resdouble.size()-1] << std::endl;
    ASSERT_TRUE(resdouble.size() == 18467);
    ralab::base::base::seq_length(100. , 1300.,19467,resdouble);
    ASSERT_TRUE(resdouble.size() == 19467);

    ralab::base::base::seq_length(0.,1000.,1000,resdouble);
    ASSERT_TRUE(resdouble.size() == 1000);
  }

  // Tests that Foo does Xyz.
  TEST_F(BaseTest, mean) {
    std::vector<double> x;
    x.push_back(1.0);
    x.push_back(1.0);
    x.push_back(1.0);
    x.push_back(1.);
    x.push_back(2.);
    x.push_back(3.);
    x.push_back(5.);
    x.push_back(5.);
    x.push_back(6.);
    x.push_back(7.);
    x.push_back(8.);
    double res = ralab::base::base::mean(x);
    ASSERT_NEAR ( 3.636364, res, 1e-4);
    res = ralab::base::base::mean(x, 0.3);
    ASSERT_NEAR ( 3.2, res, 1e-4);
    res = ralab::base::base::mean(x, 0.4);
    ASSERT_NEAR ( 3.33333, res, 1e-4);
    res = ralab::base::base::mean(x, 0.5);
    std::cout << res << std::endl;
    ASSERT_NEAR ( 3., res, 1e-4);
    res = ralab::base::base::mean(x.begin(),x.end());
    ASSERT_NEAR ( 3.636364, res, 1e-4);

  }

  TEST_F(BaseTest, geometricmean){
    std::vector<double> x;
    x.push_back(1.0);
    x.push_back(2.0);
    x.push_back(3.0);

    double res = ralab::base::base::geometricMean(x.begin(), x.end());
    ASSERT_NEAR( 1.817121, res, 1e-4 );

  }
}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
