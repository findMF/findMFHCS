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

#include <boost/shared_ptr.hpp>
#include "findmf/datastruct/maplcmsdescription.h"

namespace ralab{
  namespace findmf{
    namespace datastruct{

      struct SwathInfo{
      private:
        typedef std::map< uint32_t , MapLCMSDescriptionPtr > SwathIndic;
        SwathIndic swathindices_;

      public:
        void getKeys(std::vector<std::size_t> & keys){
          SwathInfo::SwathIndic::iterator it,end;
          it = swathindices_.begin();
          end = swathindices_.end();
          for( ; it!=end ; ++it ){
              keys.push_back(it->first);
            }
        }

        void getMapForKey(unsigned int key, MapLCMSDescriptionPtr maplcms ){
          SwathIndic::iterator it = swathindices_.find(key);
          if(it != swathindices_.end())
            {
              maplcms = it->second;
            }
        }

        /** use to update */
        MapLCMSDescriptionPtr getMapForKey(uint32_t key){
          SwathIndic::iterator it = swathindices_.find(key);
          if(it != swathindices_.end())
            {
              MapLCMSDescriptionPtr map = it->second;
              return map;
            }
          else{
              //
              std::pair<SwathIndic::iterator,bool> it = swathindices_.insert(std::make_pair(key, MapLCMSDescriptionPtr(new MapLCMSDescription())));
              return it.first->second;
            }

        }
      };

      typedef boost::shared_ptr<SwathInfo> SwathInfoPtr;
    }//
  }
}

#endif
