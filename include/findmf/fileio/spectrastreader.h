// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef SPECTRASTREADER_H
#define SPECTRASTREADER_H

#include "base/base/cpplog.h"

#include "findmf/datastruct/transition.h"
#include "swathwindowrange.h"


namespace ralab{


  ///Reads spectrast tsv file.
  class SpectrastTSVReader{
    typedef std::vector< std::vector<std::string> > StringTable;
    StringTable tableData_;
    typedef std::map<uint32_t, TransitionGroup> TranstionGroups;
    TranstionGroups transitionGroups_;
    typedef std::map<uint32_t,std::vector<uint32_t> > Swathmap;
    Swathmap swathsTransitionGroups_; // map where key is swat window and vector
    // contains ids of TransitionGroups relevant for this window.

  public:
    SpectrastTSVReader():tableData_(),transitionGroups_(){
    }

  public:
    void readtable(const std::string & filename){
      ralab::base::utils::utilities::read_table_byrow<std::string>(
            filename,
            tableData_,
            '\t'
            );
    }

    //
    // given swath window, which transitions are relevant for it.
    //
    void getTransitionsForSwath(uint32_t swath, std::vector<uint32_t> & transitions){
      Swathmap::iterator it = swathsTransitionGroups_.find((swath));
      if(it != swathsTransitionGroups_.end()){
          transitions.assign(it->second.begin(),it->second.end());
        }
    }


    void groupTransitionsBySwath(SwathWindowRange & range){
      TranstionGroups::iterator begTransG = transitionGroups_.begin();
      TranstionGroups::iterator endTransG = transitionGroups_.end();
      for(;begTransG!=endTransG;++begTransG){
          double mass = begTransG->second.PrecursorMz;
          SwathWindowRange::MCont::iterator beg2 =range.swathBoundaries_.begin();
          SwathWindowRange::MCont::iterator end2 =range.swathBoundaries_.end();
          for(;beg2 != end2 ; ++beg2){
              if(beg2->second.first < mass && mass < beg2->second.second ){
                  //LOG_INFO << beg2->second.first << " " << mass << " " << beg2->second.second ;
                  Swathmap::iterator it =  swathsTransitionGroups_.find(beg2->first);
                  if(it != swathsTransitionGroups_.end()){
                      it->second.push_back( begTransG->first);
                    }
                  else
                    {
                      std::vector<uint32_t> tmp;
                      tmp.push_back(begTransG->first);
                      swathsTransitionGroups_.insert(std::make_pair(beg2->first,tmp));
                    }
                }
            }
        }
    }

    void getTransitionGroupsIDs(std::vector<u_int32_t> & ids){
      TranstionGroups::iterator beg = transitionGroups_.begin();
      TranstionGroups::iterator end = transitionGroups_.end();
      for(;beg!=end;++beg){
          ids.push_back(beg->first);
        }
    }

    void getTranstionGroup(uint32_t groupID, TransitionGroup & t){
      TranstionGroups::iterator it = transitionGroups_.find(groupID);
      if(it != transitionGroups_.end()){
          t = it->second;
        }
    }

  public:
    void table2Groups(){
      StringTable::iterator beg = tableData_.begin();
      StringTable::iterator end = tableData_.end();
      ++beg; //move behind header
      for(;beg!=end;++beg){
          uint32_t groupID = boost::lexical_cast<uint32_t>((*beg)[0]);
          TranstionGroups::iterator it =  transitionGroups_.find(groupID);
          if(it != transitionGroups_.end()){
              Transition t;
              strings2Transtions(*beg, t);
              it->second.transitions_.push_back(t);
            }
          else
            {
              TransitionGroup tG;
              newTranstionGroup(*beg, tG);
              transitionGroups_.insert(std::make_pair(groupID,tG));
            }
        }
      tableData_.clear();
    }


  private:
    static void strings2Transtions(const std::vector<std::string> & data, Transition & t){
      t.transitionid = boost::lexical_cast<uint32_t>(data[1]);
      t.ProductMz = boost::lexical_cast<double>(data[3]);
      std::string st = data[4];
      if(st.compare("NA") == 0)
        {
          t.ProductCharge = 0.;
        }else{
          t.ProductCharge = boost::lexical_cast<uint32_t>(st);
        }
      st = data[5];
      if(st.compare("NA") == 0)
        {
          t.MassDeviation = 0.;
        }else{
          t.MassDeviation = boost::lexical_cast<double>(st);
        }
      st = data[6];
      if(st.compare("NA") == 0)
        {
          t.Tr_calibrated = false;
        }else{
          std::cout << st << "   " << data[6] << std::endl;
          t.Tr_calibrated = boost::lexical_cast<bool>(st);
        }
      t.Annotation = (data[7]);
      t.LibraryIntensity = boost::lexical_cast<float>(data[8]);
    }

  private:
    static void newTranstionGroup(const std::vector<std::string> & data,
                                  TransitionGroup & group){
      group.transitionGroupId = boost::lexical_cast<uint32_t>(data[0]);
      group.PrecursorMz = boost::lexical_cast<double>(data[2]);
      //group.swathIdx = boost::lexical_cast<uint32_t>(data[9]);
      Transition t;
      strings2Transtions(data,t);
      group.transitions_.push_back(t);
    }

  };
}

#endif
