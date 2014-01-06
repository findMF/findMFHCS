// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "findmf/datastruct/lcmsimage.h"
#include <boost/filesystem.hpp>
//#include <boost/log/trivial.hpp>

#include <iostream>
#include <vigra/impex.hxx>
#include <vigra/stdimagefunctions.hxx>
#include <vigra/convolution.hxx>

#include <vigra/watersheds.hxx>


#include "base/utils/readwritebin.h"
#include "findmf/fileio/helperfunctions.h"

namespace ralab{
  namespace findmf{
    namespace datastruct{

      void LCMSImage::put( std::size_t row, std::size_t col, float val)
      {
        (mp_)(row,col) += val;

        if((mp_)(row,col) > maxelem_){
          maxelem_ = (mp_)(row,col);
        }
      }

      float LCMSImage::getMaxelem() const{
        return maxelem_;
      }

      //TODO rename to mz or RT...
      std::size_t LCMSImage::getNrRows() const{
        return mp_.size(1);
      }

      std::size_t LCMSImage::getNrCols() const{
        return mp_.size(0);
      }

      std::size_t LCMSImage::getRTsize() const
      {
        return getNrRows();
      }

      //TODO limit image size.
      void LCMSImage::write_image(
          const std::string & filename,
          bool logb,
          const std::string & extension
          )
      {
        boost::filesystem::path p1 = addextension( filename , extension );
        vigra::MultiArray<2,float> fimage(vigra::Shape2(mp_.size(0),mp_.size(1)));
        if(logb){
          std::transform(mp_.begin(), mp_.end(), fimage.begin() , boost::bind(::log,boost::bind(std::plus<float>(),1,_1)));
        }else{
          std::transform(mp_.begin(), mp_.end(), fimage.begin() , boost::bind(::sqrt,_1));
        }
        vigra::ImageExportInfo iei(p1.string().c_str());
        vigra::exportImage(vigra::srcImageRange(fimage),iei);
      }

      /// Write transformation (calibrator data) data
      void LCMSImage::writeTransformation(const std::string & filename){
        boost::filesystem::path p2 = addextension( filename , "mass");
        ralab::base::utils::writeBin( this->bin_.getBreaks(),  p2.string());
        p2 = addextension(filename,"rt");
        ralab::base::utils::writeBin( descript_->getRT(),  p2.string());
      }

      /// writes image into tiff file + transformation
      void LCMSImage::write(const std::string & filename,
                            const std::string & extension)
      {
        boost::filesystem::path p1 = addextension( filename , extension );
        vigra::ImageExportInfo iei(p1.string().c_str());
        iei.setFileType("TIFF");
        iei.setCompression("LZW");
        iei.setPixelType( "FLOAT" ); // INT32
        vigra::exportImage(vigra::srcImageRange(mp_),iei);
        this->writeTransformation(filename);
      }

      void LCMSImage::updateMax(){
        vigra::FindMinMax<float> minmax;   // init functor
        vigra::inspectImage(srcImageRange(mp_), minmax);
        maxelem_ = minmax.max;
      }

      //you are only allowed to read vx files.
      void LCMSImage::read(const std::string & filename){
        boost::filesystem::path p1(filename);
        if(boost::filesystem::exists(p1)){
          vigra::ImageImportInfo info(p1.string().c_str());
          if(info.isGrayscale())
          {
            mp_.reshape(difference_type(info.width(), info.height()));
            vigra::importImage(info, destImage(mp_));
          }
        }

        std::vector<double> mass;
        std::string str =  p1.stem().string() ;
        str += ".mass";
        p1 = p1.parent_path();
        p1 /= str;
        ralab::base::utils::readBin(p1.string(), mass);
        bin_.setBreaks(mass);

        str =  p1.stem().string() ;
        str += ".rt";
        p1 = p1.parent_path();
        p1 /= str;
        ralab::base::utils::readBin(p1.string(), descript_->getRT() );
      }//end read image

      void LCMSImage::getSpectrum(std::size_t i,
                                  std::vector<double> & mz,
                                  std::vector<double> & intensities,
                                  double threshold
                                  )
      {
        typedef iterator CRI;
        auto begfiltered = mp_.bindOuter(i).begin();
        auto endfiltered = mp_.bindOuter(i).end();
        std::size_t dist = std::distance(begfiltered,endfiltered);
        mz.resize(dist);
        intensities.resize(dist);

        std::vector<double>::iterator begIntensOut =intensities.begin();
        std::vector<double>::iterator begMassOut = mz.begin();
        std::vector<double>::iterator mass = bin_.breaks_.begin();

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
    }

  }
}


