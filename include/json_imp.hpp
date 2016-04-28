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
#include <stdexcept>
#include <stdint.h>

namespace  cppbind {


class CppOrmException : public std::runtime_error
{
   public:
    CppOrmException(std::string const& node_name, const std::string base_msg)
        : runtime_error(node_name + " "  + base_msg)
    { }
    
    CppOrmException(const std::string base_msg)
        : runtime_error(std::string(" ")  + base_msg)
    { }
    CppOrmException(const CppOrmException& e, std::string const& node_name)
        : runtime_error(node_name + e.what())

    { }
    
};

class Mapper{
public:
    Mapper(bool isEncode, Json::Value root);
    virtual ~Mapper();
    template<typename T>
    void bind(const std::string& name, T& v, const char *default_value = NULL);
    Json::Value getJson();
private:
    bool isEncode;
    void* mapper_imp;
    //DecodeMapper decode_mapper;
    //EncodeMapper encode_mapper;
};

class DecodeMapper {
public:
    DecodeMapper(Json::Value json){
        //printf("%s\n", json.asCString());
        this->json = json;
     }
    Json::Value getJson(){
        return this->json;
    }
    template<typename T>
    void bind(const std::string& name, T& v, const char *default_value = NULL){
         //printf("filed %s\n", name.c_str());
         Json::Value jv = json[name];
         if(!jv.isNull()) {
             try {
                decode(jv, &v);
             } catch (CppOrmException e) {
                throw CppOrmException(e, std::string(".") + name);
             }
         } else  {
              throw CppOrmException(std::string(".") + name, "not found");
         }
    }

    template<typename T>
    void bind(const std::string& name, boost::shared_ptr<T>& v, const char *default_value = NULL){
        //printf("filed %s\n", name.c_str());
         Json::Value jv = json[name];
         if(!jv.isNull()) {
             T  e;
             orm(name, e);
             v = boost::shared_ptr<T>(new T(e));
         } 
    }

private: //for std container type
    template<typename T>
    void decode(const Json::Value& json, std::list<T>* e){
            if(!json.isArray()) {
                throw CppOrmException("should be a list");
            }
            for(int i = 0; i  < json.size(); i++) {
                 try { 
                    T tmp;
                    decode(json[i], &tmp);
                    e->push_back(tmp);
                 } catch  (CppOrmException e) {
                    char buf[20];
                    sprintf(buf, "[%d]", i);
                    throw CppOrmException(e, buf);
                 }
            }
    }
    template<typename T>
    void decode(const Json::Value& json, std::map<std::string, T>* e){
            if(!json.isObject()) {
                throw CppOrmException("shoulbe be a object");
            }
            Json::Value::Members keys = json.getMemberNames();
            for(Json::Value::Members::iterator it = keys.begin(); it != keys.end(); it++) {
                try { 
                    T tmp;
                    decode(json[*it], &tmp);
                    (*e)[*it] = tmp;
                } catch  (CppOrmException e) {
                   throw CppOrmException(e, std::string(".") + *it);
                }
            }
    }
private: // for class type
    template<typename T>
    void decode(const Json::Value& json, T* e){
         Mapper mapper(true, json);
       e->setBind(mapper);
    } 
private:  //for basic type
    void decode(const Json::Value& json, bool*);
    void decode(const Json::Value& json, int32_t *);
    void decode(const Json::Value& json, int64_t *);
    void decode(const Json::Value& json, float *);
    void decode(const Json::Value& json, double *);
    void decode(const Json::Value& json, std::string *);
private:
    Json::Value json;
};

class EncodeMapper {
public:
    EncodeMapper(){
    }
    Json::Value getJson(){
        return this->root;
    }
    template<typename T>
    void bind(const std::string& name, T& v, const char *default_value = NULL){
          Json::Value jv =  encode(v);
          root[name] = jv;
    }

    template<typename T>
    void bind(const std::string& name, boost::shared_ptr<T>& v, const char *default_value = NULL){
          if(v.get() != NULL) {
               Json::Value jv =  encode(*(v.get()));
               root[name] = jv;
          }
    }

private: //for std container type
    template<typename T>
    Json::Value encode(std::list<T>& e){
        Json::Value jv;
        for(typename std::list<T>::iterator  it  = e.begin(); it != e.end(); it++) {
             Json::Value je = encode(*it);
             jv.append(je);
        }
        return jv;
    }
    template<typename T>
    Json::Value encode(std::map<std::string, T>& e){
            Json::Value jv;
            for(typename std::map<std::string, T>::iterator it =  e.begin(); it != e.end(); it++) {
                Json::Value je = encode(it->second);
                jv[it->first] = je;
            }
            return jv;
    }
private: // for class type
    template<typename T>
    Json::Value encode(T& e){
       Mapper mapper(true, Json::Value());
       e.setBind(mapper);
       return mapper.getJson();
    } 
private:  //for basic type
    Json::Value encode( bool&);
    Json::Value encode( int32_t&);
    Json::Value encode( int64_t&);
    Json::Value encode( float&);
    Json::Value encode( double&);
    Json::Value encode( std::string &);
 private:
    Json::Value root;
};

 

template<typename T>
void Mapper::bind(const std::string& name, T& v, const char *default_value){
        if(this->isEncode) {
            EncodeMapper* mapper = (EncodeMapper*)this->mapper_imp;
            mapper->bind(name,v, default_value);
        } else {
            DecodeMapper* mapper = (DecodeMapper*)this->mapper_imp;
            mapper->bind(name,v, default_value);
        }
}


 

}

#endif
