#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "gtest/gtest.h"
#include "boost/date_time/posix_time/posix_time.hpp" 

namespace cppbind {
class StrTime {
private:
   std::locale locale_input;
   std::locale locale_output;
public:
    StrTime(const char *pattern = NULL) {
         if(pattern == NULL) {
             //"2005-10-15 13:14:15.003400"
             pattern = "%Y-%m-%d %H:%M:%S.%f";
         }
         locale_input = std::locale(std::locale::classic(),new boost::posix_time::time_input_facet(pattern));
         locale_output = std::locale(std::locale::classic(),new boost::posix_time::time_facet(pattern));
    }
    int parser(const std::string& timeStr, boost::posix_time::ptime *ptOut){
         boost::posix_time::ptime pt;
         std::istringstream is(timeStr);
         is.imbue(locale_input);
         //printf("%s\n", is.str().c_str());
         is >> pt;
         if(pt == boost::posix_time::ptime()) {
              return -1;
         }
         *ptOut = pt;
         return 0;
    }
    std::string format(const boost::posix_time::ptime &pt){
         std::ostringstream os;
         os.imbue(locale_output);
         os << pt;
         return os.str();
    }
};

}
