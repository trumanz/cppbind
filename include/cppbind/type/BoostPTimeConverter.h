#ifndef __CPPBIND__BoostPTimeCoverter_
#define __CPPBIND__BoostPTimeCoverter_
#include "timestr.h"

namespace cppbind{


class BoostPTimeConverter{
public:
    std::string toString(const boost::posix_time::ptime& pt) const{
        TimeStr strTime("%Y-%m-%d %H:%M:%S");
        return strTime.format(pt);
    }
    boost::posix_time::ptime fromString(const std::string& str) const{
        TimeStr strTime("%Y-%m-%d %H:%M:%S");
        boost::posix_time::ptime pt;
        int rc = strTime.parser(str, &pt);
        assert(rc == 0);
        return pt;
    }
};

class BoostTimeDurationConverter {
    public:
    std::string toString(const boost::posix_time::time_duration& dur) const{
        TimeStr strTime("%H:%M:%S");
        return strTime.format(dur);
    }
    boost::posix_time::time_duration fromString(const std::string& str) const{
        TimeStr strTime("%H:%M:%S");
        boost::posix_time::time_duration dur;
        int rc = strTime.parser(str, &dur);
        assert(rc == 0);
        return dur;
    }
};

}

#endif
