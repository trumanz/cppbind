#ifndef __CPPBIND_TIMESTR_H_
#define __CPPBIND_TIMESTR_H_

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp" 
namespace cppbind { namespace converter {

class TimeStr {
private:

   std::locale locale_input;
   std::locale locale_output;

   std::locale locale_input_4_date;
   std::locale locale_output_4_date;


   std::string pattern_4_ptime;
   std::string pattern_4_date;
public:
    TimeStr(const char *_pattern_4_ptime = NULL, const char* _pattern_4_date = NULL){
        //default "2005-10-15 13:14:15.003400"
        this->pattern_4_ptime = _pattern_4_ptime != NULL ?  _pattern_4_ptime :  "%Y-%m-%d %H:%M:%S.%f";
        this->pattern_4_date  = _pattern_4_date  != NULL ?  _pattern_4_date  :  "%Y-%m-%d";

        locale_input = std::locale(std::locale::classic(),new boost::posix_time::time_input_facet(this->pattern_4_ptime.c_str()));
        locale_output = std::locale(std::locale::classic(),new boost::posix_time::time_facet(this->pattern_4_ptime.c_str()));

        locale_input_4_date  = std::locale(std::locale::classic(),new boost::gregorian::date_input_facet(this->pattern_4_date.c_str()));
        locale_output_4_date = std::locale(std::locale::classic(),new boost::gregorian::date_facet(this->pattern_4_date.c_str()));

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
    int parser(const std::string& timeStr, boost::gregorian::date *gdOut){
         boost::gregorian::date gd;
         std::istringstream is(timeStr);
         is.imbue(locale_input_4_date);
         //printf("strtime parser %s with pattern %s\n", is.str().c_str(), this->pattern.c_str());
         is >> gd;
         //std::cout << timeStr << std::endl;
         //std::cout << gd << std::endl;
         *gdOut = gd;
         //std::cout  <<"\n" << this->format(*gdOut)  << "\n";
         if(this->format(*gdOut) != timeStr) {
              return -1;
         }
         return 0;
    }

    std::string format(const boost::gregorian::date &gd){
         std::ostringstream os;
         os.imbue(locale_output_4_date);
         os << gd;
         return os.str();
    }

    int parser(const std::string& timeStr, boost::posix_time::time_duration *ptOut){
         boost::posix_time::time_duration pt;
         std::istringstream is(timeStr);
         is.imbue(locale_input);
         //printf("strtime parser %s with pattern %s\n", is.str().c_str(), this->pattern.c_str());
         is >> pt;
         //std::cout << pt << std::endl;
         *ptOut = pt;
         //format back and check again; timeStr might 1:01:01; and str4check might be 01:01:01
         std::string str4check = this->format(*ptOut) ;
         if(timeStr[1] == ':') {
             std::string x = "0";
             x+=timeStr;
             if(str4check != x) return -1;
         } else if(str4check != timeStr) {
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

} }
#endif






 
