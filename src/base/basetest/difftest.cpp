// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "gtest/gtest.h"
#include "base/base/diff.h"
#include "base/resample/masscomparefunctors.h"

namespace {

  // The fixture for testing class Foo.
  class DiffTest : public ::testing::Test {
  protected:
    // You can remove any or all of the following functions if its body
    // is empty.
    DiffTest() {
      // You can do set-up work for each test here.
    }
    virtual ~DiffTest() {
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

  TEST_F(DiffTest, testdiff){

    double epsilon = 0.00001;
    std::vector<double> tmp, res, ref; // res- result, ref-reference
    double numbers_[] = {   2,  3,  4,  5,  6,  7,  8,  9, 10, 11 };
    const int N = sizeof( numbers_ ) / sizeof( double );
    tmp.assign( numbers_ , numbers_ + N ) ;
    ref.assign( 10 , 1 );

    ralab::base::base::diff(tmp.begin() , tmp.end() , std::back_inserter(res) , 1 );
    bool bb = std::equal( res.begin(), res.end(), ref.begin(), ralab::base::resample::DaCompFunctor<double>(epsilon) ) ;
    ASSERT_TRUE( bb );

    res.clear();
    ref.assign(10,2);
    ralab::base::base::diff(tmp.begin() , tmp.end() , std::back_inserter(res) , 2 );
    bb = std::equal( res.begin(), res.end(), ref.begin(), ralab::base::resample::DaCompFunctor<double>(epsilon) ) ;
    ASSERT_TRUE( bb );

    std::vector<double> xxx(tmp);
    std::transform( xxx.begin() , xxx.end() , xxx.begin(), xxx.begin() , std::multiplies<double>() );

    std::vector<double>::iterator itmp =
        ralab::base::base::diff
        (
          xxx.begin(),
          xxx.end(),
          3,
          3
          );
    //long long xt = std::distance(xxx.begin(),itmp);
    //xt;
    ASSERT_NEAR( *xxx.begin(),0.,  epsilon);
    ASSERT_EQ(std::distance(xxx.begin(),itmp) ,1);
  }
}//end namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
