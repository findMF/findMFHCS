// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef MyMap2XXX
#define MyMap2XXX

#include <limits>
#include <numeric>

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include <vigra/stdimage.hxx>
#include <vigra/multi_array.hxx>
#include <vigra/inspectimage.hxx>

#include "base/base/equispaceinterpolation.h"
#include "base/resample/bin1d.h"
#include "base/resample/breakspec.h"
#include "base/resample/masscomparefunctors.h"

#include "findmf/datastruct/maplcmsdescription.h"
#include "base/filter/scanfilter/getfitlertophat.h"


namespace ralab{
  namespace findmf{
    namespace datastruct{

      /**
   *Vigra multiarray with convenience methods for writing
  */
      class LCMSImage {
      public:
        typedef vigra::MultiArray<2,float> Map; // the image

      private:
        typedef Map::iterator iterator;
        typedef Map::const_iterator const_iterator;
        typedef Map::difference_type difference_type;

      private:
        Map mp_;
        float maxelem_;
        ralab::base::resample::Bin1D bin_;
        MapLCMSDescriptionPtr descript_;
        uint32_t rt2sum_;

      public :
        LCMSImage():mp_(),maxelem_( std::numeric_limits<float>::min() ),bin_(){}

        void setMapDescription(MapLCMSDescriptionPtr desc){
          descript_ = desc;
        }

        // return map description...
        MapLCMSDescriptionPtr getMapDescription(){
          // if(bin_.breaks_.size() > 0){
          descript_->mass_ = bin_.breaks_;
          //}
          return descript_;
        }


        Map & getMap(){
          return mp_;
        }

        void setMap(Map & map){
          //TODO check if size etc. are equal ...
          mp_ = map;
        }

        void updateMax();

        void resize(std::size_t nrows, std::size_t ncols) {
          mp_.reshape(Map::difference_type(nrows, ncols));
        }

        void setMZ(const std::vector<double> & mz)  {
          bin_.setBreaks(mz);
        }

        /** return mass axis */
        void getMZ(std::vector<double> & mz) const {
          bin_.getBreaks(mz);
        }

        /** */
        std::vector<double> & getRT(){
          return descript_->getRT();
        }

        double getRTforIndex(double index){
          return descript_->getRTforIndex(index);
        }

        double getMZforIndex(double index){
          return descript_->getMZforIndex(index);
        }

        void setRT(const std::vector<double> & rt)  {
          return descript_->setRT(rt);
        }

        //set and get value for index
        void put( std::size_t row, std::size_t col, float val) ;

        float get(std::size_t row, std::size_t col) const {
          float x =  mp_[difference_type(row,col)];
          return x;
        }

        std::size_t getBin(double mass) const {
          return bin_(mass);
        }

        void getBin(double mass1, double mass2, std::vector<int> & idx, std::vector<double> & weight) const {
          return bin_(mass1, mass2, idx, weight);
        }

        bool inRange(double mass) const {
          return bin_.inRange(mass);
        }

        /// get spectrum without zeros (good for writing)
        void getSpectrum(std::size_t i,
                         std::vector<double> & mz,
                         std::vector<double> & intensities ,
                         double threshold = 10) ;

        //get maximum element in map
        float getMaxelem() const ;
        //TODO rename to mz or RT...
        std::size_t getNrRows() const ;
        std::size_t getRTsize() const ;

        std::size_t getNrCols() const;
        std::size_t getMZsize() const {
          return getNrCols();
        }

        //TODO limit image dimensions.
        void write_image(
            const std::string & filename,
            bool logb = true,
            const std::string & extension = "png"
            );

        //TODO limit image dimensions.
        void write(const std::string & filename,
                   const std::string & extension = "tiff");

        void writeTransformation(const std::string & filename);


        //you are only allowed to read vx files.
        void read(const std::string & filename) ;
        //apply filter to all spectra in RT dimension


        ///
        /// computes split points of an map.
        size_t setBreak(std::pair<double, double> & mzrange, double ppm ){
          std::vector<double> breaksV;
          ralab::base::resample::PPMCompFunctor<double> ppmf(ppm);
          ralab::base::resample::breaks( mzrange.first , mzrange.second , ppmf , breaksV );
          this->setMZ(breaksV);
          return breaksV.size();
        }

      };

      typedef boost::shared_ptr<LCMSImage> LCMSImagePtr;
    }
  }

}
#endif
