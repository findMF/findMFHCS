#ifndef ISOTOPEENVELOPE_H
#define ISOTOPEENVELOPE_H

#include "base/chemistry/iisotopeenvelope.h"
#include "pwiz/utility/chemistry/IsotopeEnvelopeEstimator.hpp"
#include "pwiz/utility/chemistry/IsotopeCalculator.hpp"
#include "pwiz/utility/misc/unit.hpp"
#include "pwiz/utility/misc/Std.hpp"


namespace ralab{
  namespace findmf{
    namespace chemistry{
      struct IsotopeEnvelope : ralab::base::chemistry::IIsotopeEnvelope {

  //      using namespace pwiz::util;

        const double abundanceCutoff = .01;
        const double massPrecision = .1;
        //pwiz::chemistry::IsotopeCalculator isotopeCalculator_;

        pwiz::chemistry::IsotopeEnvelopeEstimator estimator_;

        IsotopeEnvelope(pwiz::chemistry::IsotopeEnvelopeEstimator::Config & config):estimator_(config)
        {
        }

        virtual ralab::base::chemistry::MassAbundance isotopeEnvelope(double mass) const
        {
          std::vector<double> envelope;
          pwiz::chemistry::MassDistribution xx = estimator_.isotopeEnvelope(mass);
          for( int i = 0 ; i < xx.size() ; ++i){
            envelope.push_back(xx[i].abundance);
          }
          return envelope;
        }
      };
    }
  }
}

#endif // ISOTOPEENVELOPE_H
