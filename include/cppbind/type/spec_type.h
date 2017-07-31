#ifndef __CPPBIND_SPEC_TYPE_BASE__H_
#define __CPPBIND_SPEC_TYPE_BASE__H_
#include "cppbind/cppbind_json.hpp"

namespace cppbind{

class SpecTypeBase {

public:
    void setBind(cppbind::Binder *binder){
        //printf("%s\n", binder.getJson().toStyledString().c_str());

        JsonEncodeBinder* json_encode_binder = dynamic_cast<JsonEncodeBinder*>(binder->binder_imp.get());
        JsonDecodeBinder* json_decode_binder = dynamic_cast<JsonDecodeBinder*>(binder->binder_imp.get());
        CSVDecodeBinder*  csv_decode_binder  = dynamic_cast<CSVDecodeBinder*>(binder->binder_imp.get());

        if(json_encode_binder){
             std::string value = this->encode();
             Json::Value jv(value);
             json_encode_binder->setJson(jv);
        } else if (json_decode_binder){
             std::string errmsg;
             Json::Value jv = json_decode_binder->json;
             if(!jv.isString()) {
                  throw CppBindException("should be a string");
             } 
             std::string value = jv.asString();
            int ret = this->decode(value , &errmsg);
            if(ret != 0){
                throw CppBindException(errmsg);
            }
        } else if(csv_decode_binder) {
            assert("TODO" == NULL);
        } else {
            assert("TODO" == NULL);
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
