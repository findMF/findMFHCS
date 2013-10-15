#include "findmf/algo/projectionstats.h"

#ifndef FEATURESMAPTEST_H
#define FEATURESMAPTEST_H

#include "gtest/gtest.h"
#include "findmf/algo/projectionstats.h"

// The fixture for testing class Foo.
class ProjectionStatsTest : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  ProjectionStatsTest() {
    // You can do set-up work for each test here.
  }
  virtual ~ProjectionStatsTest() {
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

TEST_F(ProjectionStatsTest, testconst){
  std::vector<float> rtproj;
  std::vector<float> mzproj;
  float rt[] = {100,120,190,220,1000,200,100};
  float mz[] = {200,220,390,420,1000,300,200};

  rtproj.assign(rt,rt+sizeof(rt)/sizeof(float));
  mzproj.assign(mz,mz+sizeof(mz)/sizeof(float));
  ralab::findmf::projstats rtp , mzp;

  for(int i = 0 ; i < 10; ++i){
      ralab::findmf::utilities::computeStats(rtproj,100,rtp);
      ralab::findmf::utilities::Pick p;
      float peakrt =  p.pick( rtproj , rtp );
      ralab::findmf::utilities::computeStats(mzproj,100,mzp);
      float peakmz =  p.pick( mzproj , mzp );
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


#endif // FEATURESMAPTEST_H
