#ifndef __CPPBIND_SPEC_TYPE_BASE__H_
#define __CPPBIND_SPEC_TYPE_BASE__H_

namespace cppbind{
class SpecTypeBase {

public:
    virtual int decode(const std::string &, std::string *errmsg) = 0;
    virtual std::string encode() = 0;
    virtual ~SpecTypeBase(){};
};

}
#endif
