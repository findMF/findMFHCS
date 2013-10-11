// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "preliminarytests.h"
#include "findmf/datastruct/swathinfo.h"
#include "findmf/fileio/spectrastreader.h"
#include "findmf/datastruct/lcmsimage.h"
#include "transitionhelpers.h"

namespace ralab{
  PreliminaryTests::PreliminaryTests(){
  }

  void PreliminaryTests::testImaging()
  {
    findmf::datastruct::LCMSImage ms1;
    ms1.read("../data/example.vx");
    SpectrastTSVReader stsv;
    stsv.readtable("../data/RCCcons_trans.txt");
    stsv.table2Groups();
    ralab::SwathWindowRange swR;
    swR.read("../data/swath32windows1.txt");
    std::vector<uint32_t> keys;
    swR.getKeys(keys);
    stsv.groupTransitionsBySwath(swR);
    findmf::datastruct::LCMSImagePtr sss = ralab::createImageForAllTranstionsInSwath(
          stsv,
          1,
          ms1,
          0.05
      );
  }

  void PreliminaryTests::testMap(){
   findmf::datastruct::LCMSImagePtr ms1 = findmf::datastruct::LCMSImagePtr(new ralab::findmf::datastruct::LCMSImage);
    ms1->read( "../data/examplems1.vx" );
    std::cout << "nr cols " <<
                 ms1->getMZsize() << "nr rows" <<
                 ms1->getRTsize() << std::endl;
    return;
  }

  void PreliminaryTests::testSpectrast(){
    ralab::SpectrastTSVReader stsv;
    stsv.readtable("../data/RCCcons_trans.txt");
    stsv.table2Groups();
    std::vector<uint32_t>  keys;
    stsv.getTransitionGroupsIDs(keys);
    ralab::TransitionGroup t;
    stsv.getTranstionGroup(keys[3],t);
    t.print(std::cout);
    t.transitions_[0].print(std::cout);
  }

  void PreliminaryTests::testSwathTable(){
    ralab::SwathWindowRange swR;
    swR.read("../data/swath32windows1.txt");
    std::vector<uint32_t> keys;
    swR.getKeys(keys);
    std::cout << swR.get(1).second << std::endl;
    std::cout << swR.get(0).second  << std::endl;
    std::cout << swR.get(40).second  << std::endl;
  }


  void PreliminaryTests::merge(){
    ralab::SpectrastTSVReader stsv;
    stsv.readtable("../data/RCCcons_trans.txt");
    stsv.table2Groups();
    ralab::SwathWindowRange swR;
    swR.read("../data/swath32windows1.txt");
    std::vector<uint32_t> keys;
    swR.getKeys(keys);
    stsv.groupTransitionsBySwath(swR);
    stsv.getTransitionsForSwath(3,keys);
    std::cout << keys.size() << std::endl;
    for(std::size_t i = 0 ; i < keys.size() ; ++i){
        ralab::TransitionGroup t;
        stsv.getTranstionGroup(keys[i],t);
        std::cout << t.PrecursorMz << std::endl;
        t.sort();
        for(std::size_t j = 0 ; j < t.transitions_.size(); ++j)
          {
            std::cout << t.transitions_[j].ProductMz << " ";
          }
        std::cout << std::endl;
      }
  }
}

int main(int argc, char *argv[])
{
  ralab::PreliminaryTests ptests;
  //ptests.merge();
  //ptests.testSwathTable();
  //ptests.testMap();
  //ptests.testImaging();
  //ptests.testSpectrast();

}
