#ifndef FEATURESMAPTEST_H
#define FEATURESMAPTEST_H

#include "gtest/gtest.h"
#include "findmf/datastruct/featuresmap.h"

// The fixture for testing class Foo.
class FeaturesMapTest : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  FeaturesMapTest() {
    // You can do set-up work for each test here.
  }
  virtual ~FeaturesMapTest() {
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

TEST_F(FeaturesMapTest, testconst){
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


#endif // FEATURESMAPTEST_H
