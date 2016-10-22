#ifndef __CPPBIND__JSONBIND_IMP__
#define __CPPBIND__JSONBIND_IMP__
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <jsoncpp/json/json.h>
#include <assert.h>
#include <list>
#include <boost/shared_ptr.hpp>
#include <stdint.h>
#include "../cppbind_exception.h"


namespace  cppbind {

class JsonBinderBase {
public:
     virtual ~JsonBinderBase() {}
     virtual const Json::Value& getJson() const = 0;
     virtual void setJson(const Json::Value &jv) = 0;
};

class Binder{
public:
    //default is encoder
    Binder();
    //decoder
    Binder(const Json::Value& root);

    /*
    *User code will call bind to build the attibute and name relation. 
    *Because the user code is not template functon, so we must tranfer a fixed type, This Binder type!!
    *and this will use EncodeBinder or DecodeBinder  to do the real work.
    */
    template<typename T>
    void bind(const std::string& name, T& v);
    Json::Value getJson();
    void setJson(const Json::Value jv);
    bool isEncode();
public:
    boost::shared_ptr<JsonBinderBase> binder_imp;
};

}

#include "imp/decode_binder.hpp"
#include "imp/encode_binder.hpp"


namespace cppbind {

template<typename T>
void Binder::bind(const std::string& name, T& v){
        //CALL the real bind fucntion, must change to the real type of binder, then comile could instantiate the tempate funciton
        EncodeBinder* encoder = NULL;
        DecodeBinder* decoder = NULL;
        if( NULL != (decoder = dynamic_cast<DecodeBinder*>(this->binder_imp.get())) ) {
            decoder->bind(name,v);
        } else if( NULL != (encoder = dynamic_cast<EncodeBinder*>(this->binder_imp.get()))) {
            encoder->bind(name,v);
        } else {
            assert("bug" == NULL);
        }
}


}

#endif
