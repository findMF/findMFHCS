#include "findmf/fileio/sqlsoci/sqlfeaturestorage.h"


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
#include "findmf/fileio/sqlsoci/sqlfeaturestorage.h"


namespace {

  class SQLSociTest: public ::testing::Test{
  protected:
    SQLSociTest(){}
  };

  TEST_F(SQLSociTest,testDBcreation)
  {
  }

  TEST_F(SQLSociTest,testCumProd)
  {
  }

  TEST_F(SQLSociTest,testCumMin)
  {

  }

  TEST_F(SQLSociTest,testCumMax)
  {

  }
}//end namespace UNITTEST

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}





