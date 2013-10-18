//#include "findmf/fileio/sqlsoci/sqlfindmfstorage.h"
// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


// Raplace ClassName with the name of the class to test...
// Test one class per unit.
//#include <iostream>
#include <vector>
#include <algorithm>
#include <gtest/gtest.h>
#include <glog/logging.h>

#include <soci/soci.h>
#include <soci/soci-sqlite3.h>
#include "base/utils/readwritelines.h"
//#include "base/utils/Copy_if.h"
#include "base/utils/split.h"
#include "findmf/utils/parsesql.h"
#include "findmf/fileio/sql/createtables.h"

namespace {

  class SQLSociTest: public ::testing::Test{
  protected:
    SQLSociTest(){
    }
    virtual void SetUp(){
    }
  };


//  TEST_F(SQLSociTest,testDBcreation)
//  {
//    std::string testfile("../sql/dbschema.sql");
//    std::vector<std::string> lines = ralab::findmf::utils::sqlparse(testfile);
//    std::string bla = "heresql.sqlite";
//    soci::session sql_( soci::sqlite3 , bla );

//    for(size_t i = 0 ; i < lines.size() ; ++i)
//      {
//        try{
//          sql_.once << lines[i];
//        }catch( std::exception const &e ){
//          LOG(INFO) << e.what() << std::endl;
//        }
//      }
//    sql_.close();
//    ASSERT_TRUE(1);
//  }

//  TEST_F(SQLSociTest,testDBcreation2)
//  {
//    std::string testfile("../sql/dbschema.sql");

//    std::string bla = "heresql2.sqlite";
//    soci::session sql_( soci::sqlite3 , bla );
//    try{
//      ralab::findmf::fileio::CreateTables(bla,testfile);
//    }catch(std::exception const& e){
//      std::cout << e.what() << std::endl;
//    }
//    ASSERT_TRUE(1);
//  }

  TEST_F(SQLSociTest, testSampleInsterts)
  {
    std::string testfile("../sql/dbschema.sql");
    std::string bla = "heresql3.sqlite";
    {
      ralab::findmf::fileio::CreateTables(bla,testfile);
    }
    //std::vector<std::string> lines = ralab::findmf::utils::sqlparse(testfile);
    ralab::findmf::datastruct::Instrument inst;
    inst.analyser = "bla";
    inst.detector = "detect";
    inst.ionisation = "ionize";
    inst.manufacturer = "flying horse";
    inst.model = "newest bit of shit";

    ralab::findmf::fileio::WriteSampleParams wsp("heresql3.sqlite");
    ralab::findmf::apps::Params params;
    params.maxmass = 1000;
    params.minintensity = 100;
    params.minmass = 10;
    params.filestem_ = "blaxbla";

    try{
    for(int i = 0 ; i < 10 ; ++i){
        uint32_t tmp = wsp.insertSample("test","greatest sample ever","test.mzML");
        std::cout << tmp << std::endl;
        wsp.insertInstrumentInfo(tmp, inst);
        wsp.insertProcessParameters(tmp, params);
      }
    }catch(std::exception & e)
    {
      std::cout << e.what() << std::endl;
    }
  }
}//end namespace UNITTEST

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}





