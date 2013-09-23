#ifndef COPY_IF_H
#define COPY_IF_H


namespace ralab{
  namespace base{
    namespace utils{

      template <class InputIterator,class InputIterator2, class OutputIterator, class Predicate>
      OutputIterator Copy_if(
          InputIterator first,
          InputIterator last,
          InputIterator2 source,
          OutputIterator result,
          Predicate pred)
      {
        while(first!=last)
          {
            if(pred(*first))
              {
                *result = *source;
                ++result;
              }
            ++first;
            ++source;
          }
        return result;
      }

      template <class InputIterator,class InputIterator2, class OutputIterator, class Predicate>
      OutputIterator Copy_if_not(
          InputIterator first,
          InputIterator last,
          InputIterator2 source,
          OutputIterator result,
          Predicate pred)
      {
        while(first!=last)
          {
            if(!pred(*first))
              {
                *result = *source;
                ++result;
              }
            ++first;
            ++source;
          }
        return result;
      }

      /*! \brief Copy_if

             Implementation of copy_if as suggested
             in Efficient STL (Scott Meyers) item 37.
            */
      template < typename InputIterator,
                 typename OutputIterator,
                 typename Predicate >
      OutputIterator Copy_if(
          InputIterator begin,
          InputIterator end,
          OutputIterator destBegin,
          Predicate p)
      {

        while(begin != end)
          {
            typename std::iterator_traits<InputIterator>::reference r= *begin;
            if(p(r))
              {
                *destBegin = r;
                ++destBegin;
              }
            ++begin;
          }
        return destBegin;
      }

      /*! \brief Copy_if_not for containers

                        Implementation of copy_if as suggested
                        in Efficient STL (Scott Meyers) item 37.
                        */
      template < typename InputIterator,
                 typename OutputIterator,
                 typename Predicate >
      OutputIterator Copy_if_not(
          InputIterator begin,
          InputIterator end,
          OutputIterator destBegin,
          Predicate p
          )
      {

        while(begin != end)
          {
            typename std::iterator_traits<InputIterator>::reference r= *begin;
            if(!p(r))
              {
                *destBegin = r;
                ++destBegin;
              }
            ++begin;
          }
        return destBegin;
      }


    }
  }
}



#endif // COPY_IF_H
