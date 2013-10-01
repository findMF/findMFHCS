//#include "findmf/fileio/sqlsoci/sqlfindmfstorage.h"
// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


// Raplace ClassName with the name of the class to test...
// Test one class per unit.
#include <vector>
#include <algorithm>
#include "gtest/gtest.h"
#include <soci/soci.h>
#include <soci/soci-sqlite3.h>
#include "base/utils/readwritelines.h"
//#include "base/utils/Copy_if.h"
#include "base/utils/split.h"
#include "findmf/utils/parsesql.h"
#include <glog/logging.h>

namespace {

  class SQLSociTest: public ::testing::Test{
  protected:
    SQLSociTest(){}
  };




  TEST_F(SQLSociTest,testDBcreation)
  {

    //std::string y = "-- ";
    std::string testfile("../sql/dbschema.sql");


    std::vector<std::string> lines = ralab::findmf::utils::sqlparse(testfile);

    std::string bla = "heresql.sqlite";
    soci::session sql_( soci::sqlite3 , bla );

    for(size_t i = 0 ; i < lines.size() ; ++i)
      {
        try{
          sql_<< lines[i];
        }catch( std::exception const &e ){
          LOG(INFO) << e.what() << std::endl;
        }
      }
    sql_.commit();
    sql_.close();
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





