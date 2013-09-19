// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


// Raplace ClassName with the name of the class to test...
// Test one class per unit.

#include "base/base/cumsum.h"

#include <vector>
#include <algorithm>
#include "gtest/gtest.h"

#include "base/base/base.h"


namespace {

  class CumSumTest: public ::testing::Test{
  protected:
    CumSumTest(){}
  };

  TEST_F(CumSumTest,testCumSum)
  {
    unsigned int resC[]={ 1 , 3 , 6 ,10 ,15 ,21 ,28 ,36 ,45 ,55};

    std::vector<unsigned int> res;
    ralab::base::base::seq(static_cast<unsigned int>(1),static_cast<unsigned int>(10),res);
    std::vector<unsigned int> res2;
    ralab::base::cumsum(res.begin(),res.end() , res2);
    std::equal(res2.begin(),res2.end(),resC);
    //test the in place version.
    ralab::base::cumsum(res.begin(),res.end());
    ASSERT_TRUE(std::equal(res.begin(),res.end(),resC));
  }

  TEST_F(CumSumTest,testCumProd)
  {
    unsigned int resC[]={1,2,6,24,120,720,5040,40320,362880,3628800};

    std::vector<unsigned int> res;
    ralab::base::base::seq(static_cast<unsigned int>(1),static_cast<unsigned int>(10),res);
    std::vector<unsigned int> res2;
    ralab::base::cumprod(res,res2);
    ASSERT_TRUE(std::equal(res2.begin(),res2.end(),resC));
  }

  TEST_F(CumSumTest,testCumMin)
  {

    unsigned int data[] ={ 3 ,2 ,1 ,2 ,1 ,0 ,4 ,3 ,2};
    unsigned int resC[]={3 , 2 , 1 , 1 , 1 , 0 , 0 , 0 , 0};

    std::vector<unsigned int> res;
    res.assign(data, data + 9);
    std::vector<unsigned int> res2;
    ralab::base::cummin(res,res2);
    ASSERT_TRUE(std::equal(res2.begin(),res2.end(),resC));
  }

  TEST_F(CumSumTest,testCumMax)
  {
    unsigned int data[] ={ 3 ,2 ,1 ,2 ,1 ,0 ,4 ,3 ,2};
    unsigned int resC[]={ 3, 3, 3, 3, 3, 3, 4, 4, 4};
    std::vector<unsigned int> res;
    res.assign(data, data + 9);
    std::vector<unsigned int> res2;
    ralab::base::cummax(res,res2);
    ASSERT_TRUE(std::equal(res2.begin(),res2.end(),resC));
  }
}//end namespace UNITTEST

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}





