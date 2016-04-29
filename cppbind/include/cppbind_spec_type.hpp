#ifndef __CPPBIND_SPEC_TYPE_BASE__H_
#define __CPPBIND_SPEC_TYPE_BASE__H_
#include "cppbind_json.hpp"

namespace cppbind{

class SpecTypeBase {

public:
    void setBind(cppbind::Binder &binder){
        //printf("%s\n", binder.getJson().toStyledString().c_str());
        if(binder.isEncode){
             std::string value = this->encode();
             Json::Value jv(value);
             binder.setJson(jv);
        } else{
             std::string errmsg;
             Json::Value jv = binder.getJson();
             if(!jv.isString()) {
                  throw CppBindException("should be a string");
             } 
             std::string value = jv.asString();
            int ret = this->decode(value , &errmsg);
            if(ret != 0){
                throw CppBindException(errmsg);
            }
        }
    }
    virtual int decode(const std::string &, std::string *errmsg) = 0;
    virtual std::string encode() = 0;
    virtual ~SpecTypeBase(){};
};

//class PosixTime

#if 0
template<typename T>
class SpecType : public T, public SpecTypeBase {
public:
    int decode(const std::string &, std::string *errmsg){
         
    }
};
#endif

}
#endif