// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "gtest/gtest.h"
#include "base/utils/readwritelines.h"

#include "base/resample/masscomparefunctors.h"
#include <boost/bind.hpp>


namespace {

  // The fixture for testing class Foo.
  class ReadLines : public ::testing::Test {
  protected:
    // You can remove any or all of the following functions if its body
    // is empty.
    ReadLines() {
      // You can do set-up work for each test here.
    }
    virtual ~ReadLines() {
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

  TEST_F(ReadLines,writeLines)
  {
    std::vector<std::string>  lines;
    lines.push_back("bla da da da");
    lines.push_back("ramba zamba");
    lines.push_back("hello world");
    ralab::base::utils::writeLines(lines,"test.txt");

    std::vector<std::string> readback;
    ralab::base::utils::readLines("test.txt",readback);
    bool test = std::equal(readback.begin(),readback.end(),lines.begin());
    ASSERT_TRUE(test);

  }

 }//namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


