// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


// Raplace ClassName with the name of the class to test...
// Test one class per unit.


#include "findmf/fileio/sql/createtables.h"
#include "findmf/fileio/sql/writesampleparams.h"

#include <vector>
#include <algorithm>
#include <gtest/gtest.h>
#include <glog/logging.h>

#include <soci/soci.h>
#include <soci/soci-sqlite3.h>
#include "base/utils/readwritelines.h"
#include "base/utils/split.h"
#include "findmf/utils/parsesql.h"

namespace {

  class SQLSociTest: public ::testing::Test{
  protected:
    SQLSociTest(){
    }
    virtual void SetUp(){
    }
  };

  TEST_F(SQLSociTest, testCreateDB)
  {
    std::string testfile("../sql/dbschema.sql");
    std::string bla = "heresql3.sqlite";
    {
      ralab::findmf::fileio::CreateTables(bla,testfile);
    }
    //std::vector<std::string> lines = ralab::findmf::utils::sqlparse(testfile);
  }

  TEST_F(SQLSociTest, testSampleInsterts)
  {
    std::string testfile("../sql/dbschema.sql");
    std::string bla = "heresqlSI.sqlite";
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
    ralab::findmf::datastruct::SampleDescript samp;
    samp.description = "very complex and important sample";
    samp.file = "/some/filelocation";
    samp.name = "test";

    ralab::findmf::apps::Params params;
    params.maxmass = 1000;
    params.minintensity = 100;
    params.minmass = 10;
    params.filestem_ = "blaxbla";


    try{
      ralab::findmf::fileio::WriteSampleParams wsp("heresqlSI.sqlite");

      for(int i = 0 ; i < 1000 ; ++i){
        params.minmass +=1;
        uint32_t tmp = wsp.write(samp,inst,params);
        //std::cout << tmp << std::endl;
      }
      wsp.commit();
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





