#ifndef __CPPBIND__BoostPTimeCoverter_
#define __CPPBIND__BoostPTimeCoverter_
#include "timestr.h"
#include "cppbind/cppbind_exception.h"

namespace cppbind{


class BoostPTimeConverter{
public:
    std::string toString(const boost::posix_time::ptime& pt) const{
        static TimeStr strTime("%Y-%m-%d %H:%M:%S");
        return strTime.format(pt);
    }
    boost::posix_time::ptime fromString(const std::string& str) const{
        static TimeStr strTime("%Y-%m-%d %H:%M:%S");
        boost::posix_time::ptime pt;
        int rc = strTime.parser(str, &pt);
        if(rc != 0) {
            throw ParseErrorException(std::string("cant not parse [") +  str + "] as time");
        }
        return pt;
    }
};


class BoostGDateConverter{
public:
    std::string toString(const boost::gregorian::date& gd) const{
        static TimeStr strTime("%Y-%m-%d");
        return strTime.format(gd);
    }
    boost::gregorian::date fromString(const std::string& str) const{
        static TimeStr strTime("%Y-%m-%d");
        boost::gregorian::date gd;
        int rc = strTime.parser(str, &gd);
        if(rc != 0) {
            throw ParseErrorException(std::string("cant not parse [") +  str + "] as date");
        }
        return gd;
    }
};


class BoostTimeDurationConverter {
    public:
    std::string toString(const boost::posix_time::time_duration& dur) const{
        static TimeStr strTime("%H:%M:%S");
        return strTime.format(dur);
    }
    boost::posix_time::time_duration fromString(const std::string& str) const{
        static TimeStr strTime("%H:%M:%S");
        boost::posix_time::time_duration dur;
        int rc = strTime.parser(str, &dur);
        if(rc != 0) {
            throw ParseErrorException(std::string("cant not parse [") +  str + "] as duration");
        }
        return dur;
    }
};

}

#endif
