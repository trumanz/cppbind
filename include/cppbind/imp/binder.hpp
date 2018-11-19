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
    template<typename T>
    T* getForeignObj(std::string key) {
        std::string type_name = typeid(T).name();
         std::map<std::string, boost::any>::iterator it = binder_data.type_tables.find(type_name);
         if(it == binder_data.type_tables.end()) {
             printf("ERROR, Please register table for %s\n", type_name.c_str());
             assert(false); 
         }
         boost::any any_table = it->second;
         const std::map<std::string, T*>* table= boost::any_cast<const std::map<std::string, T*>*>(it->second);
         assert(table != NULL);
         typename std::map<std::string, T*>::const_iterator it2 = table->find(key);
         if(it2 == table->end()) {
             for(typename std::map<std::string, T*>::const_iterator it3 = table->begin(); it3 != table->end(); it3++) {
                 printf("%s\n", it3->first.c_str());
             }
             printf("ERROR, Can not found %s in table for %s", key.c_str(), type_name.c_str());
             throw cppbind::ParseErrorException(key, "Foregin key can not found");
         }
         T* x = it2->second;
         return x;
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
    void bindDynamicType(const std::string& name, std::vector<T*>& v, Json::Value* default_value = NULL) { bindDynamicTypeImp(name, v, default_value);}
    template<typename T>
    void bindDynamicType(const std::string& name, T& v,  Json::Value* default_value = NULL) { bindDynamicTypeImp(name, v, default_value);}
private:
    template<typename T>
    void bindDynamicTypeImp(const std::string& name, T& v, Json::Value* default_value );
public:
    template<typename T>
    void bindWithDynamicTypeWithJsonDefaultValue(const std::string& name, T& v,  const char* json_str_default_value = NULL);

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
            try{
                json_decode_binder->bind(*this->json, name,v);
            }catch (cppbind::ParseErrorException& e){
                 e.addParentNodeName(name);
                 throw e;
            } catch (std::runtime_error &e) {
                 throw cppbind::ParseErrorException(name, e.what());
            } catch (std::exception &e) {
                throw cppbind::ParseErrorException(name, e.what());
            }
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
            try{
                json_decode_binder->bind(*json, name,v, default_value);
            }catch (cppbind::ParseErrorException& e){
                 e.addParentNodeName(name);
                 throw e;
            } catch (std::runtime_error &e) {
                 throw cppbind::ParseErrorException(name, e.what());
            }
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
        this->bindDynamicType(name,v,NULL);
    } else {
         Json::Value root;
         Json::Reader reader;
         bool parsingSuccessful = reader.parse(std::string(json_str_default_value), root);
         if(!parsingSuccessful) {
			 std::string err_msg =  reader.getFormattedErrorMessages();
           printf("Failed to parse, %s\n",err_msg.c_str());
           throw  ParseErrorException(err_msg);
         }
         this->bindDynamicTypeImp(name,v,&root);
    }
}

template<typename T>
void Binder::bindDynamicTypeImp(const std::string& name, T& v, Json::Value* default_value ){

    //CALL the real bind fucntion, must change to the real type of binder, then comile could instantiate the tempate funciton
    JsonEncodeBinder* json_encode_binder = dynamic_cast<JsonEncodeBinder*>(this->binder_imp);
    JsonDecodeBinder* json_decode_binder = dynamic_cast<JsonDecodeBinder*>(this->binder_imp);
    if( json_encode_binder ) {
        encoded_key.push_back(name);
        json_encode_binder->bindDynamicType(json, name,v);
    } else if(json_decode_binder ) {
        try{
          decoded_member_key_set.insert(name);
          json_decode_binder->bindDynamicType(*json, name,v, default_value );
        } catch(ParseErrorException& e) {
            e.addParentNodeName(name);
            throw e;
        }
    } 
     else {
        assert("bug" == NULL);
    }
}


}

#endif
