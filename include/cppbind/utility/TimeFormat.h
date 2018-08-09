#ifndef __CPPBIND_TIMEFORMAT_H_
#define __CPPBIND_TIMEFORMAT_H_

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp" 
#include "boost/date_time/gregorian/gregorian.hpp"

#ifdef WIN32
#define snprintf _snprintf
#endif

namespace cppbind {
class TimeFormat {
public:
    bool parse(const std::string& timeStr, boost::gregorian::date *gdOut){
         int year;
         int month;
         int day;
         int rc = sscanf(timeStr.c_str(), "%d-%d-%d", &year, &month, &day);
         if(rc != 3) return false;

         try{
             gdOut[0] = boost::gregorian::date(year, month, day);
             return true;
         } catch(std::out_of_range&) {
             return false;
         }
    }

    std::string format(const boost::gregorian::date &gd){
        char buf[100];
        int y = gd.year();
        int m = gd.month();
        int d = gd.day();
        snprintf(buf, sizeof(buf), "%d-%02d-%02d", y, m, d);
        return buf;
    }

};

}
#endif






 
