// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


//author Witold Wolski

#ifndef MAPLCMSDESCRIPTION_H
#define MAPLCMSDESCRIPTION_H

#include <utility>
#include <boost/shared_ptr.hpp>
#include "base/base/interpolate.h"

namespace ralab{
  namespace findmf{
    namespace datastruct{

      /// Description of an lcms map...
      struct MapLCMSDescription
      {
      private:
        uint32_t runnumber_; //!< running number of LCMS map
        uint32_t mslevel_; //!< ms level of the map (i.e. 1)
        typedef std::pair<double, double>  dpair;
        std::pair<double, double> extractionWindowMZ_; //!< only relevant for mslevel_ > 1 swath window
        std::pair<double, double> mzRange_; //!< mz range
        std::pair<double, double> rtRange_; //!< rt range

        std::vector<std::size_t> indices_; //!< indices of the spectra in the raw file.
        typedef std::vector<double> axesT;

        axesT retentiontime_;//!< retention times in s (index to rt)
        axesT mass_; //!< mass vector (is used convert index to mass)
        double am_; //!< vendor sampling width parameter

      public:
        /// const
        MapLCMSDescription():runnumber_(0),mslevel_(1),extractionWindowMZ_(),
          mzRange_(std::make_pair(std::numeric_limits<double>::max(),std::numeric_limits<double>::min())),
          rtRange_(std::make_pair(std::numeric_limits<double>::max(),std::numeric_limits<double>::min())),
          indices_(),retentiontime_(),mass_()
        {
        }

        std::uint32_t runnumber(){
            return runnumber_;
        }
        void runnumber(uint32_t runnumber){
            runnumber_ = runnumber;
        }
        std::uint32_t mslevel(){
            return mslevel_;
        }
        void mslevel(uint32_t mslevel){
            mslevel_ = mslevel;
        }

        dpair & extractionWindowMZ(){
            return extractionWindowMZ_;
        }
        dpair & mzRange(){
            return mzRange_;
        }
        dpair & rtRange(){
            return rtRange_;
        }

        /// use for transforming index to RT
        double getRTforIndex(double index){
          double res = 0;
          ralab::base::base::interpolate_linear( retentiontime_.begin() , retentiontime_.end() , &index,  &index+1 , &res);
          return res;
        }

        /// use for transforming index to mz
        double getMZforIndex(double index){
          double res = 0;
          ralab::base::base::interpolate_linear( mass_.begin() , mass_.end() , &index, &index+1, &res );
          return res;
        }

        std::vector<std::size_t> & getIndices(){
          return indices_;
        }

        /// get vendor sampling with
        double getAM(){
          return am_;
        }

        /// set vendor sampling with
        void setAM(double am){
          am_ = am;
        }

        /// get mass
        axesT & getMass() {
          return mass_;
        }

        /// set Mass
        void setMass(const std::vector<double> & mass){
          mass_.assign(mass.begin(),mass.end());
        }

        /// get retention time
        axesT & getRT()  {
          return retentiontime_;
        }

        /// set RT
        void setRT(const std::vector<double> & rt){
          retentiontime_.assign(rt.begin(),rt.end());
        }


        /// convinience setters
        uint32_t getMSLevel(){
          return mslevel_;
        }

        /// extraction window start
        double getMinMZsw(){
          return extractionWindowMZ_.first;
        }

        /// extraction window end
        double getMaxMZsw(){
          return extractionWindowMZ_.second;
        }

        /// min mz
        double getMinMZ(){
          return mzRange_.first;
        }
        /// max mz
        double getMaxMZ(){
          return mzRange_.second;
        }
        /// min rt
        double getMinRT(){
          return rtRange_.first;
        }

        /// max rt
        double getMaxRT(){
          return rtRange_.second;
        }


        //get the transformation vectors
        char * getRTAxis(std::size_t & size)  {
          //char * res = boost::lexical_cast<char *>(feature_.getProjectionRT());
          char * res = reinterpret_cast<char *>( &(retentiontime_[0]) );
          size = retentiontime_.size() * (sizeof(axesT::value_type)/sizeof(char));
          return res;
        }

        //get the transformation vectors
        char * getMZAxis(std::size_t & size)  {
          //char * res = boost::lexical_cast<char *>(feature_.getProjectionMZ());
          char * res = reinterpret_cast<char *>( &(mass_[0]) );
          size = mass_.size() * (sizeof(axesT::value_type)/sizeof(char));
          return res;
        }
      };//end MapLCMSDescription
      typedef boost::shared_ptr<MapLCMSDescription> MapLCMSDescriptionPtr;
    }
  }
}
#endif // MAPLCMSDESCRIPTION_H
