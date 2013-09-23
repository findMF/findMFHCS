// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "base/ms/peakpickerqtof.h"

#include "gtest/gtest.h"

#include "base/stats/normal.h"
#include "base/base/cumsum.h"
#include "base/base/base.h"
#include "base/ms/generatesamplespec.h"

namespace {

  /*!\brief Helper function to generate spectra with Gaussian peaks.
         \ingroup FILTER
         */

  // The fixture for testing class Foo.
  class PeakPickerTest : public ::testing::Test {
  protected:
    std::vector<double> data , dcumsum , mass, spec;
    // You can remove any or all of the following functions if its body
    // is empty.
    PeakPickerTest() {
      // You can do set-up work for each test here.
    }
    virtual ~PeakPickerTest() {
      // You can do clean-up work that doesn't throw exceptions here.
    }
    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp() {
      // Code here will be called immediately after the constructor (right
      // before each test).

      typedef double value_type;
      std::size_t specsize=10000;

      ralab::base::stats::rnorm( specsize , data );
      ralab::base::cumsum(data.begin(),data.end() , dcumsum );
      ralab::base::base::seq_length(300.,1000.,dcumsum.size(),mass);
      // Code here will be called immediately after the constructor (right
      // before each test).


      ralab::base::ms::generateSampleSpec(specsize,3.,200,spec);
    }

    virtual void TearDown() {
      // Code here will be called immediately after each test (right
      // before the destructor).
    }

    // Objects declared here can be used by all tests in the test case for Foo.
  };

  // Tests that the Foo::Bar() method does Abc.
  TEST_F(PeakPickerTest, testpicker) {

    double resolution = 10000;
    std::pair<double, double> massrange = std::make_pair(mass.front(),mass.back());
    ralab::base::ms::PeakPicker<double, ralab::base::ms::SimplePeakArea> pp(resolution, massrange);
    pp(mass.begin(),mass.end(),spec.begin());
    std::cout << "nrPeaks" << pp.getPeakArea().size() << std::endl;
  }
}//end namespace


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

