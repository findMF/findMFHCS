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

        pwiz::chemistry::IsotopeEnvelopeEstimator estimator_;
        double abundanceCutoff;
        double massPrecision;
        //pwiz::chemistry::IsotopeCalculator isotopeCalculator_;



        IsotopeEnvelope(pwiz::chemistry::IsotopeEnvelopeEstimator::Config & config):estimator_(config),
          abundanceCutoff(.01),massPrecision(.1)
        {
        }

        virtual ralab::base::chemistry::MassAbundance isotopeEnvelope(double mass) const
        {
          std::vector<double> envelope;
          pwiz::chemistry::MassDistribution xx = estimator_.isotopeEnvelope(mass);
          for( uint32_t i = 0 ; i < xx.size() ; ++i){
            envelope.push_back(xx[i].abundance);
          }
          return envelope;
        }
      };
    }
  }
}

#endif // ISOTOPEENVELOPE_H
