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

}

#endif
