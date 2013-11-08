#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <string>

namespace ralab{
  namespace findmf{
    namespace datastruct{
      //dto to tansfer instrument information.

      struct SampleDescript{
        std::string  name;
        std::string description;
        std::string file;
      };

      struct Instrument{
        std::string manufacturer;
        std::string model;
        std::string ionisation;
        std::string analyser;
        std::string detector;
      };

      

    }
  }

}
#endif
