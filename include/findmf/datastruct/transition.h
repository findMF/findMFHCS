// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


//author Witold Wolski


#ifndef TRANSITION_H
#define TRANSITION_H
#include <boost/filesystem.hpp>


#include "base/utils/readwritelines.h"
#include "base/utils/readtable.h"
#include <map>


namespace ralab{

  struct Transition{
    uint32_t transitionid;
    double ProductMz;
    double ProductCharge; //TODO check this.... i gues unsigned int suffices
    double MassDeviation;
    double Tr_calibrated;
    std::string Annotation;
    float LibraryIntensity;

    std::ostream & print(std::ostream & o){
      o << "Printing Transtion: " << std::endl;
      o << "transitionid     :" << transitionid << std::endl;
      o << "ProductMz        :" << ProductMz << std::endl;
      o << "ProductCharge    :" << ProductCharge << std::endl;
      o << "MassDeviation    :" << MassDeviation <<std::endl;
      o << "Tr_calibrated    :" << Tr_calibrated << std::endl;
      o << "Annotation       :" << Annotation << std::endl;
      o << "LibraryIntensity :" << LibraryIntensity << std::endl;
      return o;
    }
  };

  struct TranstionMassCompareFunctor:std::binary_function<Transition,Transition,bool>{
    bool operator()(const Transition & a, const Transition & b)const{
      return a.ProductMz < b.ProductMz;
    }
  };//

  struct TransitionGroup{
    uint32_t transitionGroupId;
    double PrecursorMz;
    std::vector< Transition > transitions_;
    void sort(){
      std::sort(transitions_.begin(),transitions_.end(), TranstionMassCompareFunctor());
    }
    //
    // get transiton mz's
    //
    void getTransitionMZ(std::vector<double> & masses) const {
      masses.resize(transitions_.size());
      for(std::size_t j = 0 ; j < transitions_.size() ; ++j){
          masses[j] = transitions_[j].ProductMz;
        }
    }

    std::ostream & print(std::ostream & o){
      o << "Printing Transition Group: " << std::endl;
      o << "transitionGroupId : " << transitionGroupId << std::endl;
      o << "PrecursorMz       : " << PrecursorMz << std::endl;
      o << "nr transitions    : " << transitions_.size() << std::endl;
      return o;
    }
  };

  struct TranstionGroupMassCompareFunctor:std::binary_function<TransitionGroup,TransitionGroup,bool>{
    bool operator()(TransitionGroup & a, TransitionGroup & b){
      return a.PrecursorMz > b.PrecursorMz;
    }
  };//

  /** Helper struct which sorts transitions by mass */
  struct TRansitionGroupsMass{
    typedef std::multimap<double, TransitionGroup> TranstionGroupsD;
    TranstionGroupsD transitionGroups_;

    void set(std::map<uint32_t, TransitionGroup> & transgroups){
      typedef std::map<uint32_t, TransitionGroup> Uintmap;
      Uintmap::iterator beg = transgroups.begin();
      Uintmap::iterator end = transgroups.end();
      for(;beg!=end;++beg){
          TransitionGroup tg = end->second;
          transitionGroups_.insert(std::make_pair(tg.PrecursorMz,tg ));
        }
    }
  };//TRansitionGroupsMass


}//end namespace ralab

#endif // SPECTRASTREADER_H
