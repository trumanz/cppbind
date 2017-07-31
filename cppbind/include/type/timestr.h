#ifndef __CPPBIND_TIMESTR_H_
#define __CPPBIND_TIMESTR_H_

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp" 
namespace cppbind {
class TimeStr {
private:
   std::locale locale_input;
   std::locale locale_output;
   std::string pattern;
public:
    TimeStr(const char *pattern_for_ptime = NULL){
        if(pattern_for_ptime == NULL) {
            this->pattern = "%Y-%m-%d %H:%M:%S.%f";// "2005-10-15 13:14:15.003400"
        } else {
            this->pattern = pattern_for_ptime;
        }
        locale_input = std::locale(std::locale::classic(),new boost::posix_time::time_input_facet(this->pattern.c_str()));
        locale_output = std::locale(std::locale::classic(),new boost::posix_time::time_facet(this->pattern.c_str()));
    }
    int parser(const std::string& timeStr, boost::posix_time::ptime *ptOut){
         boost::posix_time::ptime pt;
         std::istringstream is(timeStr);
         is.imbue(locale_input);
         //printf("strtime parser %s with pattern %s\n", is.str().c_str(), this->pattern.c_str());
         is >> pt;
         //std::cout << pt << std::endl;
         if(pt == boost::posix_time::ptime()) {
              return -1;
         }
         *ptOut = pt;
         if(this->format(*ptOut) != timeStr) {
              return -1;
         }
         return 0;
    }
    std::string format(const boost::posix_time::ptime &pt){
         std::ostringstream os;
         os.imbue(locale_output);
         os << pt;
         return os.str();
    }

    int parser(const std::string& timeStr, boost::posix_time::time_duration *ptOut){
         boost::posix_time::time_duration pt;
         std::istringstream is(timeStr);
         is.imbue(locale_input);
         printf("strtime parser %s with pattern %s\n", is.str().c_str(), this->pattern.c_str());
         is >> pt;
         std::cout << pt << std::endl;
         *ptOut = pt;
         if(this->format(*ptOut) != timeStr) {
              return -1;
         }
         return 0;
    }

    std::string format(const boost::posix_time::time_duration &pt){
         std::ostringstream os;
         os.imbue(locale_output);
         os << pt;
         return os.str();
    }
};

}
#endif






 
