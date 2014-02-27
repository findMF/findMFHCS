// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


//author Witold Wolski


#ifndef _SWATHBOOKKEEPER__
#define _SWATHBOOKKEEPER__

#include <iostream>
#include <iterator>
#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include "findmf/datastruct/maplcmsdescription.h"

namespace ralab{
  namespace findmf{
    namespace datastruct{

      struct MSFileInfo{
      private:
        std::string filename_; //!< name of file needs to include path
        typedef std::map< uint32_t , MapLCMSDescriptionPtr > SwathIndic;
        SwathIndic swathindices_; //!< descriptions of the maps

      public:

        MSFileInfo():filename_(){}
        MSFileInfo(std::string filename):filename_(filename){}

        ///vector containing id's (starting mass of window as int) of the lcms maps.
        void getKeys(std::vector<std::size_t> & keys){
          MSFileInfo::SwathIndic::iterator it,end;
          it = swathindices_.begin();
          end = swathindices_.end();
          for( ; it!=end ; ++it ){
              keys.push_back(it->first);
            }
        }

        /// get the filename
        std::string getFilename(){
         return boost::filesystem::path(filename_).stem().string();
        }

        /// set the filename
        std::string getPath(){
          boost::filesystem::path p = boost::filesystem::absolute(filename_);
          p = p.parent_path();
          return p.string();
        }


        /// get map description for key
        void getMapDescriptionForKey(unsigned int key, MapLCMSDescriptionPtr maplcms ){
          SwathIndic::iterator it = swathindices_.find(key);
          if(it != swathindices_.end())
            {
              maplcms = it->second;
            }
        }

        ///  map descritipion given key
        MapLCMSDescriptionPtr getMapDescriptionForKey(uint32_t key){
          SwathIndic::iterator it = swathindices_.find(key);
          if(it != swathindices_.end())
            {
              MapLCMSDescriptionPtr map = it->second;
              map->runnumber_ = key;
              return map;
            }
          else{
              std::pair<SwathIndic::iterator,bool> it = swathindices_.insert(std::make_pair(key, MapLCMSDescriptionPtr(new MapLCMSDescription())));
              return it.first->second;
            }

        }
      };

      typedef boost::shared_ptr<MSFileInfo> MSFileInfoPtr;
    }//
  }
}

#endif
