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
    void isotopeEnvelope(double mass, ralab::base::chemistry::MassAbundance & massabundance) const override
    {
      typedef std::pair<double, double> MassAbund;
      MassAbund p1, p2, p3, p4, p5, p6;
      p1.first = 1699.64684;
      p1.second = 35.22;
      p2.first = 1700.64970;
      p2.second = 33.75;
      p3.first = 1701.65163;
      p3.second = 19.21;
      p4.first = 1702.65331;
      p4.second = 8.07;
      p5.first = 1703.65495;
      p5.second = 2.73;
      p6.first = 1704.65663;
      p6.second = 0.78;

      massabundance.push_back(35.22);
      massabundance.push_back(33.75);
      massabundance.push_back(19.21);
      massabundance.push_back(8.07);
      massabundance.push_back(2.73);
      massabundance.push_back(0.78);

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
    bla->isotopeEnvelope(1700., blue);
    ASSERT_NEAR(blue[0], 35.22 , 1e-7);
  }




}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
