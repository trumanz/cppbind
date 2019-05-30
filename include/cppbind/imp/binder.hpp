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
#include "cppbind/Object.h"
namespace  cppbind {

BOOST_TTI_HAS_MEMBER_FUNCTION(setBind)
BOOST_TTI_HAS_MEMBER_FUNCTION(fromJsonValue4Bind)
BOOST_TTI_HAS_MEMBER_FUNCTION(toJsonValue4Bind)

template<typename T>
class SimpleMapTable{
  const std::map<std::string, T*>* table;
public:
  SimpleMapTable(const std::map<std::string, T*>* _table) { this->table = _table;}
  T* getObj(std::string key){
    typename std::map<std::string, T*>::const_iterator it = table->find(key);
    if(it == table->end()) {
     printf("ERROR, Can not found %s", key.c_str());
     throw cppbind::ParseErrorException(key, "Foregin key can not found");
    }
    T* x = it->second;
    return x;
  }
};

class ForeignTableInterface {
 public:
  virtual boost::any getAnyObj(std::string key) = 0;
};

template<typename ObjT, typename TableT>
class ForeginTableT : public ForeignTableInterface{
  boost::shared_ptr<TableT> table;
public:
  ForeginTableT(boost::shared_ptr<TableT> _table) {
    this->table = _table;
  }
  boost::any getAnyObj(std::string key){
    ObjT* obj =  table->getObj(key);
    boost::any x = obj;
    return x;
  }

};



class BinderData{ 
public:
    StringConverterManager str_convert_mgmt;
    std::map<std::string, boost::shared_ptr<ForeignTableInterface> > type_tables;
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

     template<typename ObjT>
     void regTable(const std::map<std::string, ObjT*> *table)
     {
         boost::shared_ptr<SimpleMapTable<ObjT> > t(new SimpleMapTable<ObjT>(table));
         this->regTable<ObjT, SimpleMapTable<ObjT> >(t);
     }
     template<typename ObjT, typename FTable>
     void regTable(boost::shared_ptr<FTable> ft){
       this->regTableInterface<ObjT>(new ForeginTableT<ObjT,FTable>(ft));
     }
 private:
     template<typename ObjT>
     void regTableInterface(ForeignTableInterface* tabe){
       binder_data.type_tables[ typeid(ObjT).name()] =  boost::shared_ptr<ForeignTableInterface>(tabe);
     }
 public:
     
  
     void regClassRegister(ClassRegisterBase* _class_reg){
        assert(binder_data.class_reg == NULL);
        binder_data.class_reg = _class_reg;
    }
    template<typename T>
    T* getForeignObj(std::string key) {
      const std::string type_name = typeid(T).name();
      std::map<std::string, boost::shared_ptr<ForeignTableInterface> >::iterator it = binder_data.type_tables.find(type_name);
       if(it == binder_data.type_tables.end()) {
           printf("ERROR, Please register table for %s\n", type_name.c_str());
           assert(false); 
       }
       boost::any any_obj = it->second->getAnyObj(key);
       T* obj = boost::any_cast<T*>(any_obj);
       if (obj == NULL) {
         printf("can not find foreign obj %s", key.c_str());
         assert(false); 
       }
       return obj;
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
    void bindDynamicType(const std::string& name, T &v,  Json::Value* default_value = NULL) { bindDynamicTypeImp(name, v, default_value);}
    template<typename T>
    void bindDynamicType(const std::string& name, T &v,  Json::Value default_value) { bindDynamicTypeImp(name, v, &default_value);}
    template<typename T>
    void bindDynamicType(const std::string& name, boost::shared_ptr<T> &v, boost::shared_ptr<T> default_value) { bindDynamicTypeImp(name, v, default_value);}
private:
    template<typename T>
    void bindDynamicTypeImp(const std::string& name, T &v, Json::Value* default_value );
    template<typename T>
    void bindDynamicTypeImp(const std::string& name, boost::shared_ptr<T> &v, boost::shared_ptr<T> default_value );

private:
	template<typename T>
	void addEncodedKey(const std::string& key, const boost::shared_ptr<T>& v)
	{
		if (v.get())
			encoded_key.push_back(key);
	}
    template<typename T>
	void addEncodedKey(const std::string& key, const T& v) { encoded_key.push_back(key); }
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
            //encoded_key.push_back(name);
			addEncodedKey(name, v);
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
void Binder::bindDynamicTypeImp(const std::string& name, T &v, Json::Value* default_value ){

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


template<typename T>
void Binder::bindDynamicTypeImp(const std::string& name, boost::shared_ptr<T> &v, boost::shared_ptr<T> default_value )
{
    //CALL the real bind fucntion, must change to the real type of binder, then comile could instantiate the tempate funciton
    JsonEncodeBinder* json_encode_binder = dynamic_cast<JsonEncodeBinder*>(this->binder_imp);
    JsonDecodeBinder* json_decode_binder = dynamic_cast<JsonDecodeBinder*>(this->binder_imp);
    if( json_encode_binder ) {
        encoded_key.push_back(name);
        T* e = v.get();
        if (e) {
            json_encode_binder->bindDynamicType(json, name, e);
        }
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
