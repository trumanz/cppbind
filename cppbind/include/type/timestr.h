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
    TimeStr(const char *pattern = NULL);
    int parser(const std::string& timeStr, boost::posix_time::ptime *ptOut);
    std::string format(const boost::posix_time::ptime &pt);

    int parser(const std::string& timeStr, boost::posix_time::time_duration *ptOut);
    std::string format(const boost::posix_time::time_duration &pt);
};

}
