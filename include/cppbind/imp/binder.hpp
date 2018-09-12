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
#include <boost/tti/has_member_function.hpp>
#include <stdint.h>
#include "../cppbind_exception.h"
#include "cppbind/type/StringConverter.h"
#include "cppbind/type/BoostPTimeConverter.h"
namespace  cppbind {

BOOST_TTI_HAS_MEMBER_FUNCTION(setBind)
BOOST_TTI_HAS_MEMBER_FUNCTION(fromJsonValue4Bind)
BOOST_TTI_HAS_MEMBER_FUNCTION(toJsonValue4Bind)



class BinderData{
public:
    StringConverterManager str_convert_mgmt;
    std::map<std::string, boost::any> type_tables;
    ClassRegisterBase* class_reg;
public:
    BinderData(){
        this->class_reg = NULL;
        str_convert_mgmt.addStringConverter<boost::posix_time::ptime,BoostPTimeConverter>();
        str_convert_mgmt.addStringConverter<boost::posix_time::time_duration,BoostTimeDurationConverter>();
        str_convert_mgmt.addStringConverter<boost::gregorian::date,BoostGDateConverter>();
    }
};

class BinderImpBase {
public:
     virtual ~BinderImpBase() {}
     BinderData binder_data;

     template<typename T>
     void regTable(const std::map<std::string, T*> *table)
     {
         std::string type_name = typeid(T).name();
         binder_data.type_tables[type_name] = table;
     }
     void regClassRegister(ClassRegisterBase* _class_reg){
        assert(binder_data.class_reg == NULL);
        binder_data.class_reg = _class_reg;
    }
};

class Binder{
public:
    BinderImpBase* binder_imp;
    Json::Value *json;
    std::set<std::string> decoded_member_key_set;
    std::vector<std::string> encoded_key;
    Binder() {}
    void init(BinderImpBase* _binder_imp, Json::Value* jv){
        this->binder_imp = _binder_imp;
        this->json  = jv;
        this->encoded_key.clear();
        this->decoded_member_key_set.clear();
        //if(jv != NULL) {
        //    std::cout << __FILE__ << __LINE__  <<  this->json[0] << "\n";
        //}
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

//foreign key
public:
    template<typename T>
    void bindForeginKey(const std::string& name, T& v) { bindForeginKey(name,v, (const T*)NULL);}
    template<typename T>
    void bindForeginKey(const std::string& name, T& v, const T& default_value) { bindForeginKey(name,v,&default_value);}
private:
    template<typename T>
    void bindForeginKey(const std::string& name, T& v, const T* default_value);
public:
    template<typename T>
    void bindWithDynamicType(const std::string& name, T& v,  Json::Value* default_value = NULL);

    template<typename T>
    void bindWithDynamicTypeWithJsonDefaultValue(const std::string& name, T& v,  const char* json_str_default_value = NULL);
    
    template<typename T>
    void bindDynamicTypeArray(const std::string& name, std::vector<T*>& v);
};

}

#include "cppbind/imp/json_decode_binder.hpp"
#include "cppbind/imp/json_encode_binder.hpp"
#include "cppbind/imp/CSVDecodeBinder.hpp"


namespace cppbind {

template<typename T>
void Binder::bind(const std::string& name, T& v){
        //CALL the real bind fucntion, must change to the real type of binder, then comile could instantiate the tempate funciton
        JsonEncodeBinder* json_encode_binder = dynamic_cast<JsonEncodeBinder*>(this->binder_imp);
        JsonDecodeBinder* json_decode_binder = dynamic_cast<JsonDecodeBinder*>(this->binder_imp);
        if( json_encode_binder ) {
            encoded_key.push_back(name);
            json_encode_binder->bind(json,name,v);
        } else if(json_decode_binder ) {
            decoded_member_key_set.insert(name);
            json_decode_binder->bind(*this->json, name,v);
        } 
         else {
            assert("bug" == NULL);
        }
}


template<typename T>
void Binder::bind(const std::string& name, T& v, const T& default_value){
        //CALL the real bind fucntion, must change to the real type of binder, then comile could instantiate the tempate funciton
        JsonEncodeBinder* json_encode_binder = dynamic_cast<JsonEncodeBinder*>(this->binder_imp);
        JsonDecodeBinder* json_decode_binder = dynamic_cast<JsonDecodeBinder*>(this->binder_imp);
        if( json_encode_binder ) {
            encoded_key.push_back(name);
            json_encode_binder->bind(json, name,v, default_value);
        } else if(json_decode_binder ) {
            decoded_member_key_set.insert(name);
            json_decode_binder->bind(*json, name,v, default_value);
        } 
         else {
            assert("bug" == NULL);
        }
}

template<typename T>
void Binder::bindForeginKey(const std::string& name, T& v, const T* default_value){
        //CALL the real bind fucntion, must change to the real type of binder, then comile could instantiate the tempate funciton
        JsonEncodeBinder* json_encode_binder = dynamic_cast<JsonEncodeBinder*>(this->binder_imp);
        JsonDecodeBinder* json_decode_binder = dynamic_cast<JsonDecodeBinder*>(this->binder_imp);
        if( json_encode_binder ) {
            encoded_key.push_back(name);
            json_encode_binder->bindForeginKey(json, name,v, default_value);
        } else if(json_decode_binder ) {
            decoded_member_key_set.insert(name);
            json_decode_binder->bindForeginKey(*json, name,v, default_value);
        } 
         else {
            assert("bug" == NULL);
        }
}


template<typename T>
void Binder::bindWithDynamicTypeWithJsonDefaultValue(const std::string& name, T& v,  const char* json_str_default_value)
{
    if(json_str_default_value==NULL) {
        this->bindWithDynamicType(name,v,NULL);
    } else {
         Json::Value root;
         Json::Reader reader;
         bool parsingSuccessful = reader.parse(std::string(json_str_default_value), root);
         if(!parsingSuccessful) {
			 std::string err_msg =  reader.getFormattedErrorMessages();
           printf("Failed to parse, %s\n",err_msg.c_str());
           throw  CppBindException(err_msg);
         }
         this->bindWithDynamicType(name,v,&root);
    }
}

template<typename T>
void Binder::bindWithDynamicType(const std::string& name, T& v, Json::Value* default_value ){

    //CALL the real bind fucntion, must change to the real type of binder, then comile could instantiate the tempate funciton
    JsonEncodeBinder* json_encode_binder = dynamic_cast<JsonEncodeBinder*>(this->binder_imp);
    JsonDecodeBinder* json_decode_binder = dynamic_cast<JsonDecodeBinder*>(this->binder_imp);
    if( json_encode_binder ) {
        encoded_key.push_back(name);
        json_encode_binder->bindWithDynamicType(json, name,v);
    } else if(json_decode_binder ) {
        decoded_member_key_set.insert(name);
        json_decode_binder->bindWithDynamicType(*json, name,v, default_value );
    } 
     else {
        assert("bug" == NULL);
    }
}


template<typename T>
void Binder::bindDynamicTypeArray(const std::string& name, std::vector<T*>& v)
{
    //CALL the real bind fucntion, must change to the real type of binder, then comile could instantiate the tempate funciton
    JsonEncodeBinder* json_encode_binder = dynamic_cast<JsonEncodeBinder*>(this->binder_imp);
    JsonDecodeBinder* json_decode_binder = dynamic_cast<JsonDecodeBinder*>(this->binder_imp);
    if( json_encode_binder ) {
        encoded_key.push_back(name);
        json_encode_binder->bindDynamicTypeArray(json, name,v);
    } else if(json_decode_binder ) {
        try{
           decoded_member_key_set.insert(name);
           json_decode_binder->bindDynamicTypeArray(*json, name,v);
        } catch(CppBindException& e) {
            throw CppBindException(e, std::string(".") + name);
        }
    } 
     else {
        assert("bug" == NULL);
    }
}

}

#endif
