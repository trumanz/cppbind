#ifndef __CPPBIND__JSONBIND_IMP__
#define __CPPBIND__JSONBIND_IMP__
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <json/json.h>
#include <assert.h>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/tti/has_member_function.hpp>
#include <stdint.h>
#include "cppbind_exception.h"
#include "cppbind/type/StringConverter.h"
#include "cppbind/type/BoostPTimeConverter.h"
#include "cppbind/Object.h"
namespace  cppbind {

BOOST_TTI_HAS_MEMBER_FUNCTION(setBind)
BOOST_TTI_HAS_MEMBER_FUNCTION(fromJsonValue4Bind)
BOOST_TTI_HAS_MEMBER_FUNCTION(toJsonValue4Bind)

#include "ForeignTableInterface.h"

#include "EnconderDecoderBase.h"

class JsonEncoderImp;
class JsonDecoderImp;

//
class BinderParameter {

};

class Binder{
public:
    JsonEncoderImp* encoder;
    JsonDecoderImp* decoder;
    Json::Value *json;
    std::set<std::string> decoded_member_key_set;
    std::vector<std::string> encoded_key;
    Binder(EnconderDecoderBase* _binder_imp, Json::Value* jv);

    /*
    *User code will call bind to build the attibute and name relation. 
    *Because the user code is not template functon, so we must tranfer a fixed type, This Binder type!!
    *and this will use EncodeBinder or DecodeBinder  to do the real work.
    */

    template<typename T>
    void bind(T& v, const T* default_value = NULL);
    template<typename T>
    void bind(const std::string& name, T& v, const T* default_value = NULL);
    template<typename T>
    void bind(const std::string& name, T& v, const T& default_value) { bind(name,v,&default_value);}
    template<typename T>
    void bind(const std::string& name, boost::shared_ptr<T>& v);

//foreign key
public:
    //template<typename T>
    //void encodeForeignKey(const std::string& name, T& v) { decodeForeignKey(name,v, (const T*)NULL);}
    template<typename T>
    void bindForeginKey(const std::string& name, T& v, const T& default_value) { bindForeginKey(name,v,&default_value);}
//private:
    template<typename T>
    void bindForeginKey(const std::string& name, T& v, const T* default_value = nullptr);
    template<typename T, typename GetKey_CallT>
    void bindForeginKey(const std::string& name, T& v, GetKey_CallT& call);
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

#include "cppbind/imp/decoder/JsonDecoderImp.hpp"
#include "cppbind/imp/JsonEncoderImp.hpp"
#include "cppbind/imp/CSVDecodeBinder.hpp"


namespace cppbind {

inline Binder::Binder(EnconderDecoderBase* _binder_imp, Json::Value* jv){
    this->decoder = nullptr;
    this->encoder = nullptr;
    this->encoder = dynamic_cast<JsonEncoderImp*>(_binder_imp);
    this->decoder = dynamic_cast<JsonDecoderImp*>(_binder_imp);
    this->json  = jv;
    this->encoded_key.clear();
    this->decoded_member_key_set.clear();
}

template<typename T>
void Binder::bind(T& v, const T* default_value) {
        if( encoder ) {
            Json::Value jv;
            encoder->encode(v, &jv);
            this->json[0] = jv;
        } else if(decoder ) {
            try{
              if (!this->json[0].isNull()) {
                  decoder->decode(this->json[0], &v);
              } else if(default_value) {
                v = *default_value;
              } else {
                throw ParseErrorException(std::string("not found"));
              }
            }catch (cppbind::ParseErrorException& e){
                 throw e;
            } catch (std::exception &e) {
                 throw cppbind::ParseErrorException("", e.what());
            }
        } 
         else {
            assert("bug" == NULL);
        }
}

template<typename T>
void Binder::bind(const std::string& name, T& v, const T* default_value){
        if( encoder ) {
            encoded_key.push_back(name);
            Json::Value jv;
            encoder->encode(v, &jv);
            this->json[0][name] = jv;
        } else if(decoder ) {
            decoded_member_key_set.insert(name);
            try{
              if (this->json->isMember(name) && !this->json[0][name].isNull()) {
                  decoder->decode(this->json[0][name], &v);
              } else if(default_value) {
                v = *default_value;
              } else {
                throw ParseErrorException(std::string("not found"));
              }
            }catch (cppbind::ParseErrorException& e){
                 e.addParentNodeName(name);
                 throw e;
            } catch (std::exception &e) {
                 throw cppbind::ParseErrorException(name, e.what());
            }
        } 
         else {
            assert("bug" == NULL);
        }
}


template<typename T>
void Binder::bind(const std::string& name, boost::shared_ptr<T>& v)
{
  if( encoder) {
    if (v.get()) {
      encoded_key.push_back(name);
      Json::Value jv;
      encoder->encode(v.get()[0], &jv);
      this->json[0][name] = jv;
    }
  } else if(decoder) {
    decoded_member_key_set.insert(name);
    if(this->json->isMember(name) && !this->json[0][name].isNull() ) {
      const Json::Value& jv = this->json[0][name];
       T  e;
       decoder->decode(jv, &e);
       v = boost::shared_ptr<T>(new T(e));
    }
  } else {
    assert("bug" == NULL);
  }
}

template<typename T>
void Binder::bindForeginKey(const std::string& name, T& v, const T* default_value){
  JsonEncoderImp* json_encode_binder = encoder;
  JsonDecoderImp* json_decode_binder = decoder;
        if( json_encode_binder ) {
            encoded_key.push_back(name);
            json_encode_binder->encodeForeignKey(json, name, v, default_value);
        } else if(json_decode_binder ) {
            decoded_member_key_set.insert(name);
            json_decode_binder->decodeForeignKey(*json, name, v, default_value);
        } 
         else {
            assert("bug" == NULL);
        }
}

template<typename T, typename GetKey_CallT>
    void Binder::bindForeginKey(const std::string& name, T& v, GetKey_CallT& call){
        JsonEncoderImp* json_encode_binder = encoder;
        JsonDecoderImp* json_decode_binder = decoder;
        if( json_encode_binder ) {
            encoded_key.push_back(name);
            json_encode_binder->encodeForeignKey(json, name, v, call);
        } else if(json_decode_binder ) {
            decoded_member_key_set.insert(name);
            json_decode_binder->decodeForeignKey(*json, name, v, (T *) nullptr);
        }
        else {
            assert("bug" == NULL);
        }
}


template<typename T>
void Binder::bindDynamicTypeImp(const std::string& name, T &v, Json::Value* default_value ){
  JsonEncoderImp* json_encode_binder = encoder;
  JsonDecoderImp* json_decode_binder = decoder;
    if( json_encode_binder ) {
        encoded_key.push_back(name);
        json_encode_binder->bindDynamicType(json, name,v);
    } else if(json_decode_binder ) {
        try{
          decoded_member_key_set.insert(name);
            json_decode_binder->decodeDynamicType(*json, name, v, default_value);
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
  JsonEncoderImp* json_encode_binder = encoder;
  JsonDecoderImp* json_decode_binder = decoder;
    if( json_encode_binder ) {
        encoded_key.push_back(name);
        T* e = v.get();
        if (e) {
            json_encode_binder->bindDynamicType(json, name, e);
        }
    } else if(json_decode_binder ) {
        try{
          decoded_member_key_set.insert(name);
            json_decode_binder->decodeDynamicType(*json, name, v, default_value);
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
