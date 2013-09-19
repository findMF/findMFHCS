#ifndef PREPAREDATA_H
#define PREPAREDATA_H

namespace ralab{
  namespace base{
    namespace filter{
      namespace utilities{
        /*!\brief

       Sequence: 1 2 3 4 5 , and filter width 5 and mirror false than res will be:
       4 5 1 2 3 4 5 1 2,
       if mirror true than:
       2 1 1 2 3 4 5 5 4

       */
        template <typename TContainer ,typename TIterator>
        typename TContainer::iterator prepareData
        (
            TIterator dataBeg ,
            TIterator dataEnd ,
            size_t fsize,
            TContainer &res , //!< [out]
            bool mirror = false //!< should it be circular or mirrored.
            )
        {
          if(mirror)
            {
              typename TContainer::iterator it;
              size_t fsize2 = (fsize-1)/2;
              res.resize(std::distance(dataBeg,dataEnd)+fsize);
              boost::reverse_iterator<TIterator> reverse_begin(dataEnd);
              boost::reverse_iterator<TIterator> reverse_end(dataBeg);

              it = std::copy(reverse_end - fsize2,reverse_end, res.begin() );
              it = std::copy(dataBeg,dataEnd, it );
              it = std::copy( reverse_begin, reverse_begin + fsize2, it);
              return it;
            }
          else
            {
              typename TContainer::iterator it;
              size_t fsize2 = (fsize-1)/2;
              res.resize(std::distance(dataBeg,dataEnd)+fsize);
              it = std::copy(dataEnd - fsize2,dataEnd, res.begin() );
              it = std::copy(dataBeg,dataEnd, it );
              it = std::copy( dataBeg, dataBeg + fsize2, it);
              return it;
            }
        }
      }//end utilities

    }//filter
  }//base
}//ralab

#endif // PREPAREDATA_H
