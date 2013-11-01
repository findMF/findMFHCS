// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "base/chemistry/iisotopeenvelope.h"


#include <gtest/gtest.h>


// a sample implementation of the IisotopeEnvelope interface
namespace ralab{
  struct IsotopeEnvelope : ralab::base::chemistry::IIsotopeEnvelope
  {
    IsotopeEnvelope(){}
    //just a dummy implementation
    ralab::base::chemistry::MassAbundance isotopeEnvelope(double mass) const override
    {
      ralab::base::chemistry::MassAbundance mab;
      mab.push_back(35.22);
      mab.push_back(33.75);
      mab.push_back(19.21);
      mab.push_back(8.07);
      mab.push_back(2.73);
      mab.push_back(0.78);

      return mab;
    }

  };

}

namespace {

  // The fixture for testing class Foo.
  class IsotopeEnvelopeTest : public ::testing::Test {
  protected:
    std::vector<double> data , dcumsum , res , res2;

    // You can remove any or all of the following functions if its body
    // is empty.
    IsotopeEnvelopeTest() {
      // You can do set-up work for each test here.
    }
    virtual ~IsotopeEnvelopeTest() {
    }
  };

  // Tests that the Foo::Bar() method does Abc.
  TEST_F(IsotopeEnvelopeTest, testEnvelope) {
    ralab::base::chemistry::IIsotopeEnvelope * bla = new ralab::IsotopeEnvelope;
    ralab::base::chemistry::MassAbundance blue;
    blue = bla->isotopeEnvelope(1700.);
    ASSERT_NEAR(blue[0], 35.22 , 1e-7);
  }




}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
