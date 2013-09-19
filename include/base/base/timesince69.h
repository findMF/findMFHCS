// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef TIMESINCE69_H
#define TIMESINCE69_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace ralab
{
  namespace base
  {
      inline void getDurationSince69(boost::posix_time::time_duration & td)
      {
        using namespace boost::posix_time;
        boost::gregorian::date d(1969,boost::gregorian::Feb,24);
        boost::posix_time::ptime pd(d);
        boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
        //boost::gregorian::date today = now.date();
        td = now - pd;
      }

      inline uint32_t secondsSince69()
      {
        boost::posix_time::time_duration td;
        getDurationSince69(td);
        return (td.hours()*3600 + td.minutes()*60 + td.seconds());
      }

      inline uint32_t minutesSince69()
      {
        boost::posix_time::time_duration td;
        getDurationSince69(td);
        return (td.hours()*60 + td.minutes());
      }
  }//base
}//ralab

#endif
