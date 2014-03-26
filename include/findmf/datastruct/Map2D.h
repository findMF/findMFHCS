#ifndef MAP2D_H
#define MAP2D_H

#include <boost/filesystem.hpp>
#include <boost/bind.hpp>

#include <vigra/multi_array.hxx>
#include <vigra/stdimage.hxx>
#include <vigra/inspectimage.hxx>
#include <vigra/impex.hxx>
#include <vigra/stdimagefunctions.hxx>
#include <vigra/convolution.hxx>

#include "findmf/fileio/helperfunctions.h"

namespace ralab{
  namespace findmf{
    namespace datastruct{

      /// Vigra multiarray wrapped
      template<typename Tmap>
      struct Map2D{

        typedef typename vigra::MultiArray<2,Tmap> Map;
        typedef typename Map::difference_type difference_type;

      private:
        Map map_; //!< map
        float maxelem_; //!< largest element in map?
        float minelem_; //!< smallest element in map

      public:
        Map2D():map_(),maxelem_( std::numeric_limits<float>::min() ){}

        /// get a map
        Map & getMap(){
          return map_;
        }

        /// set a map
        void setMap(Map & map){
          map_ = map;
        }

        /// update maximum variable
        void updateImageRange(){
          vigra::FindMinMax<Tmap> minmax;   // init functor
          vigra::inspectImage(srcImageRange(map_), minmax);
          maxelem_ = minmax.max;
          minelem_ = minmax.min;
        }

        Tmap getImageMax(){
          return maxelem_;
        }

        Tmap getImageMin(){
          return minelem_;
        }


        /// get image value at index
        float get(std::size_t row, std::size_t col) const {
          float x =  map_[difference_type(row,col)];
          return x;
        }

        /// put a value into an image
        void put( std::size_t row, std::size_t col, float val)
        {
          (map_)(row,col) += val;

          if((map_)(row,col) > maxelem_){
            maxelem_ = (map_)(row,col);
          }
        }

        /// resize image map
        void resize(std::size_t nrows, std::size_t ncols) {
          map_.reshape(difference_type(nrows, ncols));
        }

        /// write image into png
        void write_image(
            const std::string & filename,
            bool logb = true,
            const std::string & extension = "png"
            ){
          boost::filesystem::path p1 = addextension( filename , extension );
          vigra::MultiArray<2,float> fimage(vigra::Shape2(map_.size(0),map_.size(1)));
          if(logb){
            std::transform(map_.begin(), map_.end(), fimage.begin() , boost::bind(::log,boost::bind(std::plus<float>(),1,_1)));
          }else{
            std::transform(map_.begin(), map_.end(), fimage.begin() , boost::bind(::sqrt,_1));
          }
          vigra::ImageExportInfo iei( p1.string().c_str() );
          vigra::exportImage( vigra::srcImageRange(fimage) , iei);
        }


        /// write image into tiff file
        void write(
            const std::string & outdir,
            const std::string & filename,
            const std::string & extension = "tiff")
        {
          boost::filesystem::path p1(outdir);
          boost::filesystem::path p2 = addextension( filename , extension );
          p1 /= p2;
          vigra::ImageExportInfo iei(p1.string().c_str());
          iei.setFileType("TIFF");
          //iei.setCompression("LZW");
          iei.setPixelType( "FLOAT" ); // INT32
          vigra::exportImage(vigra::srcImageRange(map_),iei);
        }

        /// read a map
        void read(const std::string & filename){
          boost::filesystem::path p1(filename);
          if(boost::filesystem::exists(p1)){
            vigra::ImageImportInfo info(p1.string().c_str());
            if(info.isGrayscale())
            {
              //map_.reshape(Map::difference_type(info.width(), info.height()));
              vigra::importImage(info, destImage(map_));
            }
          }
        }
      }; // end class Map2D
    }//datastruct
  }//findmf
}//ralab


#endif // MAP2D_H
