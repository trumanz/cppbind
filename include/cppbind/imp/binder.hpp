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
#include "cppbind/type/StringConverter.h"
#include "cppbind/type/BoostPTimeConverter.h"
namespace  cppbind {

BOOST_TTI_HAS_MEMBER_FUNCTION(setBind)
BOOST_TTI_HAS_MEMBER_FUNCTION(fromJsonValue4Bind)
BOOST_TTI_HAS_MEMBER_FUNCTION(toJsonValue4Bind)

class BinderImpBase {
public:
     virtual ~BinderImpBase() {}
};

class Binder{
public:
    /*  
    //default is encoder
    Binder();
    //decoder
    Binder(const Json::Value& root);
    Binder(const std::vector<std::string>& csv_row); 
    */ 
    Binder(boost::shared_ptr<BinderImpBase> binder_imp){
        this->binder_imp = binder_imp;
        str_convert_mgmt.addStringConverter<boost::posix_time::ptime,BoostPTimeConverter>();
        str_convert_mgmt.addStringConverter<boost::posix_time::time_duration,BoostTimeDurationConverter>();
        str_convert_mgmt.addStringConverter<boost::gregorian::date,BoostGDateConverter>();
    }
    /*
    *User code will call bind to build the attibute and name relation. 
    *Because the user code is not template functon, so we must tranfer a fixed type, This Binder type!!
    *and this will use EncodeBinder or DecodeBinder  to do the real work.
    */
    template<typename T>
    void bind(const std::string& name, T& v);
    template<typename T>
    void bind(const std::string& name, T& v, const T& default_value);
    template<typename T>
    void bindWithForeginKey(const std::string& name, T& v);
 public:
    boost::shared_ptr<BinderImpBase> binder_imp;
    StringConverterManager str_convert_mgmt;
};

}

#include "cppbind/imp/json_decode_binder.hpp"
#include "cppbind/imp/json_encode_binder.hpp"
#include "cppbind/imp/CSVDecodeBinder.hpp"


namespace cppbind {

template<typename T>
void Binder::bind(const std::string& name, T& v){
        //CALL the real bind fucntion, must change to the real type of binder, then comile could instantiate the tempate funciton
        JsonEncodeBinder* json_encode_binder = dynamic_cast<JsonEncodeBinder*>(this->binder_imp.get());
        JsonDecodeBinder* json_decode_binder = dynamic_cast<JsonDecodeBinder*>(this->binder_imp.get());
        if( json_encode_binder ) {
            json_encode_binder->bind(name,v);
        } else if(json_decode_binder ) {
            json_decode_binder->bind(name,v);
        } 
         else {
            assert("bug" == NULL);
        }
}


template<typename T>
void Binder::bind(const std::string& name, T& v, const T& default_value){
        //CALL the real bind fucntion, must change to the real type of binder, then comile could instantiate the tempate funciton
        JsonEncodeBinder* json_encode_binder = dynamic_cast<JsonEncodeBinder*>(this->binder_imp.get());
        JsonDecodeBinder* json_decode_binder = dynamic_cast<JsonDecodeBinder*>(this->binder_imp.get());
        if( json_encode_binder ) {
            json_encode_binder->bind(name,v, default_value);
        } else if(json_decode_binder ) {
            json_decode_binder->bind(name,v, default_value);
        } 
         else {
            assert("bug" == NULL);
        }
}

template<typename T>
void Binder::bindWithForeginKey(const std::string& name, T& v){
        //CALL the real bind fucntion, must change to the real type of binder, then comile could instantiate the tempate funciton
        JsonEncodeBinder* json_encode_binder = dynamic_cast<JsonEncodeBinder*>(this->binder_imp.get());
        JsonDecodeBinder* json_decode_binder = dynamic_cast<JsonDecodeBinder*>(this->binder_imp.get());
        if( json_encode_binder ) {
            json_encode_binder->bindWithForeginKey(name,v);
        } else if(json_decode_binder ) {
            json_decode_binder->bindWithForeginKey(name,v);
        } 
         else {
            assert("bug" == NULL);
        }
}

}

#endif
