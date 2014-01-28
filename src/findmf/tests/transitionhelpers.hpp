// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef TRANSITIONHELPERS_H
#define TRANSITIONHELPERS_H

#include "findmf/fileio/spectrastreader.h"
#include "findmf/datastruct/lcmsimage.h"

namespace ralab
{
  inline ralab::findmf::datastruct::LCMSImagePtr makeImageforTranstion(
      ralab::TransitionGroup t,
      ralab::findmf::datastruct::LCMSImage & map,
      double acc
      )
  {
    t.sort();
    ralab::findmf::datastruct::LCMSImagePtr out = ralab::findmf::datastruct::LCMSImagePtr(new ralab::findmf::datastruct::LCMSImage());
    {
      std::vector<double> x;
      map.getMZ(x);
      out->setMZ(x);
    }

    //TODO test if there is a bug here.
    for(std::size_t j = 0 ; j < t.transitions_.size(); ++j)
      {
        std::cout <<t.transitions_[j].ProductMz << " "
                  <<  t.transitions_[j].ProductMz - acc/2.
                  << " " << t.transitions_[j].ProductMz + acc/2. <<  std::endl;
      }
    return out;
  }

  inline ralab::findmf::datastruct::LCMSImagePtr createImageForAllTranstionsInSwath(
      ralab::findmf::SpectrastTSVReader & stsv,
      uint32_t swath,
      ralab::findmf::datastruct::LCMSImage & map,
      double acc
      )
  {
    std::vector<uint32_t> keys;
    stsv.getTransitionsForSwath(swath , keys);
    ralab::findmf::datastruct::LCMSImagePtr out = ralab::findmf::datastruct::LCMSImagePtr(new ralab::findmf::datastruct::LCMSImage());
    {
      std::vector<double> x;
      map.getMZ(x);
      out->setMZ(x);
    }

    std::cout << keys.size() << std::endl;
    for(int i = 0 ; i < 1  ; ++i){
        ralab::TransitionGroup t;
        stsv.getTranstionGroup(keys[i],t);
        ralab::findmf::datastruct::LCMSImagePtr tmp = makeImageforTranstion(t,map,acc);
      }
    return out;
  }
}

#endif // TRANSITIONHELPERS_H
