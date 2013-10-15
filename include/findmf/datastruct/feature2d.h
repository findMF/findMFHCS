// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


//author Witold Wolski

#ifndef FEATURE2D_H
#define FEATURE2D_H

#include <vector>
#include "findmf/algo/projectionstats.h"

namespace ralab
{
  namespace findmf
  {
    namespace datastruct
    {
      /** */
      struct Feature2D{
        typedef float value_type;

      private:
        bool problem_;

        std::vector<value_type> mzprojection_;
        std::vector<value_type> rtprojection_;
        unsigned int id_;
        //
        float minMZsw_;
        float maxMZsw_;

        uint32_t swathid_;

        std::size_t minMZIndex_; // start of feature in mz
        std::size_t minRTIndex_; // start of feature in rt

        value_type sum_; //total intensity
        std::size_t count_; //number of pixels in features
        value_type max_; //maximum intensity in feature

        value_type centerOfMassMZ_;
        value_type centerOfMassRT_;

        value_type maxLocationMZ_;
        value_type maxLocationRT_;

        ralab::findmf::projstats mzstats_;
        ralab::findmf::projstats rtstats_;

      public:

        Feature2D():problem_(false),id_(0),minMZsw_(),maxMZsw_(),swathid_(),minMZIndex_(0),minRTIndex_(0),sum_(0.),count_(0.)
        ,max_(0.),centerOfMassMZ_(0.),centerOfMassRT_(0.),maxLocationMZ_(0.),
          maxLocationRT_(0.)
        {}


        Feature2D(std::size_t mzMinIndex,
                  std::size_t mzExtend,
                  std::size_t rtMinIndex,
                  std::size_t rtExtend):
          mzprojection_(mzExtend,0.), rtprojection_(rtExtend,0.),
          minMZIndex_(mzMinIndex),minRTIndex_(rtMinIndex),
          sum_(0.),count_(0),max_(0.),centerOfMassMZ_(0.),
          centerOfMassRT_(0.),maxLocationMZ_(0.),maxLocationRT_(0.)
        {
        }

        void setProblem(bool p){
          problem_ = p;
        }

        bool getProblem(){
          return problem_;
        }

        /**/
        void setID(unsigned int id){
          id_ = id;
        }

        /**/
        unsigned int getID()const {
          return id_;
        }

        unsigned int getSwathId() const {
          return swathid_;
        }

        void setSwathId(uint32_t swid){
          swathid_ = swid;
        }

        //swath window min mz
        float getMinMZsw() const{
          return minMZsw_;
        }

        void setMinMZsw(float mz){
          minMZsw_ = mz;
        }

        //swath window max mz
        float getMaxMZsw() const{
          return maxMZsw_;
        }

        void setMaxMZsw(float mz){
          maxMZsw_ = mz;
        }

        void setVolume(value_type val) {
          sum_ = val;
        }

        value_type getVolume() const{
          return sum_;
        }

        void setMaximum(value_type val){
          max_ = val;
        }

        value_type getMaximum() const{
          return max_;
        }

        void setCount(std::size_t val){
          count_ = val;
        }

        std::size_t getCount() const {
          return count_;
        }

        //center of mass
        void setCenterOfMassMZ(float val ) {
          centerOfMassMZ_ = val;
        }

        value_type getCenterOfMassMZ() const{
          return centerOfMassMZ_;
        }

        void setCenterOfMassRT(float val) {
          centerOfMassRT_ = val;
        }

        value_type getCenterOfMassRT() const {
          return centerOfMassRT_;
        }

        // maximum location
        void setMaxLocationMZ(float val){
          maxLocationMZ_ = val;
        }

        value_type getMaxLocationMZ() const{
          return maxLocationMZ_;
        }

        void setMaxLocationRT(float val){
          maxLocationRT_ = val;
        }

        value_type getMaxLocationRT()const {
          return maxLocationRT_;
        }

        void add(std::size_t posMz, std::size_t posRt , value_type val){
          mzprojection_[posMz-minMZIndex_] += val;
          rtprojection_[posRt-minRTIndex_] += val;
        }

        int getMinRTIdx()const {
          return minRTIndex_;
        }

        void setMinRTIdx(std::size_t val){
          minRTIndex_= val;
        }

        int getRTExtend() const{
          return rtprojection_.size();
        }

        int getMinMZIdx()const {
          return minMZIndex_;
        }

        int getMZExtend() const{
          return mzprojection_.size();
        }

        std::vector<value_type> & getProjectionMZ(){
          return mzprojection_;
        }

        void setProjectionMZ(std::vector<float> & mz){
          mzprojection_ = mz;
        }

        void setProjectionRT(std::vector<float> & rt){
          rtprojection_ = rt;
        }

        std::vector<value_type> & getProjectionRT(){
          return rtprojection_;
        }

        // calls an mz peak finder behind the scenes
        ralab::findmf::projstats & getRTStats() {
          return rtstats_;
        }

        //calls an mz peak finder behind the scenes
        ralab::findmf::projstats & getMZStats() {
          return mzstats_;
        }
      };//end

    }
  }

}//end ralab
#endif // FEATURE2D_H
