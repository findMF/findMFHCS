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
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>


#include "findmf/fileio/helperfunctions.h"
#include "base/resample/convert2dense.h"
#include "base/resample/determinebinwidth.h"

#include "base/utils/readwritebin.h"

#include "findmf/datastruct/maplcmsdescription.h"
#include "findmf/datastruct/Map2D.h"
#include "findmf/datastruct/msfileinfo.h"

namespace ralab{
  namespace findmf{
    namespace datastruct{

      /// lcms image
      class LCMSImage {
      public:
        typedef Map2D<float> FloatMap;

      private:
        ralab::findmf::datastruct::MSFileInfoPtr sip_;//!< reference to file info
        FloatMap imageMap_; //!< the original image
        ralab::base::resample::Convert2Dense c2d_; //!< 2 dense converter

        MapLCMSDescriptionPtr descript_; //!< map desctipion
        uint32_t rt2sum_; //!< nr of spektra to average
        ralab::base::resample::SamplingWith sw_;
      public :
        LCMSImage():sip_(new ralab::findmf::datastruct::MSFileInfo()),
          imageMap_(),c2d_(),descript_(new MapLCMSDescription()),rt2sum_(1)
        {}

        LCMSImage(ralab::findmf::datastruct::MSFileInfoPtr sip):sip_(sip),imageMap_(),
          descript_(new MapLCMSDescription()),rt2sum_(1)
        {}

        /// set swath info pointer
        void setSIP(ralab::findmf::datastruct::MSFileInfoPtr sip){
          sip_ = sip;
        }

        /// return swath info pointer of this map
        ralab::findmf::datastruct::MSFileInfoPtr getSip(){
          return sip_;
        }

        /// set the map description
        void setMapDescription(MapLCMSDescriptionPtr desc){
          descript_ = desc;
        }

        /// return map description...
        MapLCMSDescriptionPtr getMapDescription(){
          descript_->setMass( c2d_.bin_.breaks_ );
          return descript_;
        }

        /// access image map
        FloatMap & getImageMap(){
          return imageMap_;
        }

        /// set the image map
        void setImageMap(FloatMap & map){
          //TODO check if size etc. are equal ...
          imageMap_ = map;
        }

        /// computes split points of an map.
        size_t defBreak(std::pair<double, double> & mzrange, double ppm ){
          return c2d_.defBreak(mzrange,ppm );
        }

        /// set the mz axis
        void setMZ(const std::vector<double> & mz)  {
          c2d_.bin_.setBreaks(mz);
        }

        /// return mass axis
        void getMZ(std::vector<double> & mz) const {
          c2d_.bin_.getBreaks(mz);
        }

        /// return rt axis
        std::vector<double> & getRT() {
          return descript_->getRT();
        }

        /// retention time at image index
        double getRTforIndex(double index){
          return descript_->getRTforIndex(index);
        }

        /// mass at image index
        double getMZforIndex(double index){
          return descript_->getMZforIndex(index);
        }

        /// set retention time axis
        void setRT(const std::vector<double> & rt)  {
          return descript_->setRT(rt);
        }

        /// set and get value for index
        void put( std::size_t row, std::size_t col, float val) {
          imageMap_.put(row, col, val);
        }

        /// Converts a sparse spec to a dense spec
        void convert2dense(std::vector<double> & mz,
                           std::vector<double> & intens,
                           std::size_t specnr
                           //datastruct::LCMSImage & map_
                           ){
          c2d_.setAm( sw_(mz.begin(),mz.end()));
          c2d_.convert2dense(mz.begin(),mz.end(),
                             intens.begin(),
                             imageMap_.getMap().bindOuter(specnr).begin()
                             );
        }

        /// determine the pixel coordinate of a mass
        std::size_t getBin(double mass) const {
          return c2d_.bin_(mass);
        }

        /// determine the pixel corrdinates of a mass range (mass1 and mass2).
        void getBin(double mass1, double mass2,
                    std::vector<int> & idx, std::vector<double> & weight) const {
          c2d_.getBin(mass1, mass2, idx, weight);
        }

        /// check if mass is within range
        bool inRange(double mass) const {
          return c2d_.bin_.inRange(mass);
        }


        /// access dimensions of image
        std::size_t getNrRows()  {
          return imageMap_.getMap().size(1);
        }
        /// access RT dimensions of LCMS map
        std::size_t getRTsize()  {
          return getNrRows();
        }


        /// access dimensions of image
        std::size_t getNrCols() {
          return imageMap_.getMap().size(0);
        }
        /// access MZ dimensions of LCMS map
        std::size_t getMZsize()  {
          return getNrCols();
        }

        /// read the image and transformators
        void read(const std::string & filename){
          imageMap_.read(filename);
          this->readTransformation(filename);
        }

        /// write the image and transformators
        /// gets the file location information from the sip
        void write(){
          std::string path = sip_->getPath();
          std::string filename = sip_->getFilename();
          int runnum = this->descript_->runnumber_;
          filename += "_" + boost::lexical_cast<std::string>(runnum);

          imageMap_.write(path , filename);
          this->writeTransformation(sip_->getPath(),sip_->getFilename());
        }


        /// write the image and transformators
        /// gets the file location information from the sip
        void write(std::string &outdir){
          //std::string path = sip_->getPath();
          std::string filename = sip_->getFilename();
          int runnum = this->descript_->runnumber_;
          filename += "_" + boost::lexical_cast<std::string>(runnum);

          imageMap_.write(  outdir , filename);
          this->writeTransformation(outdir,sip_->getFilename());
        }


        /// write the image
        void write(std::string path, std::string filename){
          imageMap_.write(path, filename);
          this->writeTransformation(path,filename);
        }


        /// given RT index return spectrum
        void getSpectrum(std::size_t i,
                         std::vector<double> & mz,
                         std::vector<double> & intensities,
                         double threshold = 10 //!< minimal intensity still to retrieve
            )
        {
          auto begfiltered = imageMap_.getMap().bindOuter(i).begin();
          auto endfiltered = imageMap_.getMap().bindOuter(i).end();
          std::size_t dist = std::distance(begfiltered,endfiltered);
          mz.resize(dist);
          intensities.resize(dist);

          std::vector<double>::iterator begIntensOut = intensities.begin();
          std::vector<double>::iterator begMassOut = mz.begin();
          std::vector<double>::iterator mass = c2d_.bin_.breaks_.begin();

          std::size_t countLarger = 0;
          std::size_t countAll = 0;
          for(; begfiltered != endfiltered; ++begfiltered, ++mass, ++countAll){
            if(*begfiltered > threshold)//skip values zero
            {
              ++countLarger;
              *begIntensOut = *begfiltered;
              *begMassOut = *mass;
              ++begMassOut;
              ++begIntensOut;
            }
          }
          mz.resize(countLarger);
          intensities.resize(countLarger);
        }





      private:

        /// write the transformators (mapping of RT and mz to pixel coordinates into file)
        void writeTransformation(const std::string outdir, const std::string & filename){
          boost::filesystem::path p1(outdir);
          std::string workname = filename + "_" + boost::lexical_cast<std::string>(this->descript_->runnumber_);
          boost::filesystem::path p2 = addextension( workname ,  "mass" );
          p1 /= p2;
          ralab::base::utils::writeBin( this->c2d_.bin_.getBreaks(),  p1.string());
          boost::filesystem::path p3(outdir);
          p2 = addextension(workname,"rt");
          p3 /=p2;
          ralab::base::utils::writeBin( descript_->getRT(),  p3.string());
        }


        void readTransformation(const std::string & filename){
          boost::filesystem::path p1(filename);
          std::vector<double> mass;
          std::string str =  p1.stem().string() ;
          str += ".mass";
          p1 = p1.parent_path();
          p1 /= str;
          ralab::base::utils::readBin(p1.string(), mass);
          c2d_.bin_.setBreaks(mass);

          str =  p1.stem().string() ;
          str += ".rt";
          p1 = p1.parent_path();
          p1 /= str;
          ralab::base::utils::readBin(p1.string(), descript_->getRT() );
        }//end read image

      };

      typedef boost::shared_ptr<LCMSImage> LCMSImagePtr;
    }
  }

}
#endif
