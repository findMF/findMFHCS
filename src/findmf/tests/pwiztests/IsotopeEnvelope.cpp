//
// $Id: IsotopeEnvelopeEstimatorTest.cpp 4129 2012-11-20 00:05:37Z chambm $
//
//
// Original author: Darren Kessner <darren@proteowizard.org>
//
// Copyright 2007 Spielberg Family Center for Applied Proteomics
//   Cedars Sinai Medical Center, Los Angeles, California  90048
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//


#include "pwiz/utility/chemistry/IsotopeEnvelopeEstimator.hpp"
#include "pwiz/utility/chemistry/IsotopeCalculator.hpp"
#include "pwiz/utility/misc/unit.hpp"
#include "pwiz/utility/misc/Std.hpp"
#include <cstring>

using namespace pwiz::util;
using namespace pwiz::chemistry;

ostream* os_ = 0;


void test()
{
    const double abundanceCutoff = .01;
    const double massPrecision = .1;
    IsotopeCalculator isotopeCalculator(abundanceCutoff, massPrecision);

    IsotopeEnvelopeEstimator::Config config;
    config.isotopeCalculator = &isotopeCalculator;

    IsotopeEnvelopeEstimator estimator(config);

    //std::vector<double> tmp;
    //std::vector<std::vector< double > > bb;
    if (os_)
        for (int mass=100; mass<=8000; mass+=10)
          {
            MassDistribution xx = estimator.isotopeEnvelope(mass);
            //*os_ << mass << ":\n" << xx << endl;
            *os_ <<  mass;

            for( unsigned int i = 0 ; i < 8 ; ++i ){
                if(i < xx.size()){
                    *os_ <<  "\t" << xx[i].abundance;
                  }
                else
                  {
                    *os_ << "\t" << 0.;
                  }
              }
             *os_ << std::endl;
          }
    /// TODO: external verification of these estimates
}


int main(int argc, char* argv[])
{
    TEST_PROLOG(argc, argv)

    try
    {
        ofstream myfile;
        myfile.open ("example.txt");
        if (argc>1 && !strcmp(argv[1],"-v")) os_ = &myfile;
        //if (os_) *os_ << "IsotopeEnvelopeEstimatorTest" << std::endl;
        test();
    }
    catch (exception& e)
    {
        TEST_FAILED(e.what())
    }
    catch (...)
    {
        TEST_FAILED("Caught unknown exception.")
    }
    TEST_EPILOG
}
