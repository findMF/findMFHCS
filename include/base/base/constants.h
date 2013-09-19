// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

/*! \file Constants.h
\brief Constants

*/

namespace ralab
{
  namespace constants
  {

      /* \brief the ratio of the circumference of a circle to its diameter;*/
      const double PI(3.14159265358979323846264338327950288);
      /*! \brief  the three-letter abbreviations for the English month names; */
      static const char * month_abb[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
      /*! \brief the English names for the months of the year; */
      static const char * month_name[12] =  { "January", "February", "March", "April" , "May" , "June", "July", "August", "September", "October",  "November", "December"};
      /*! \brief the 26 upper-case letters of the Roman alphabet; */
      const char LETTERS[26] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K','L', 'M', 'N',
                                 'O', 'P','Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y' ,'Z'};
      /*! \brief the 26 lower-case letters of the Roman alphabet; */
      const char letters[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
                                 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
      /*!*/

  }//end algo
}//end ralab


#endif // CONSTANTS_H
