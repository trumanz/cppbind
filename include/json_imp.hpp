#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <jsoncpp/json/json.h>
#include <assert.h>
#include <list>
#include <boost/shared_ptr.hpp>
#include <stdexcept>

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

//TODO is_encode not used, just for decode
class Mapper {
public:
    Mapper(Json::Value json, bool is_encode){
        this->json = json;
        this->is_encode = is_encode;
    }

    template<typename T>
    void orm(const std::string& name, T& v, const char *default_value = NULL){
         printf("filed %s\n", name.c_str());
         Json::Value jv = json[name];
         if(!jv.isNull()) {
             try {
                get(jv, &v);
             } catch (CppOrmException e) {
                throw CppOrmException(e, std::string(".") + name);
             }
         } else  {
              throw CppOrmException(std::string(".") + name, "not found");
         }
    }

    template<typename T>
    void orm(const std::string& name, boost::shared_ptr<T>& v, const char *default_value = NULL){
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
    void get(const Json::Value& json, std::list<T>* e){
            if(!json.isArray()) {
                throw CppOrmException("should be a list");
            }
            for(int i = 0; i  < json.size(); i++) {
                 try { 
                    T tmp;
                    get(json[i], &tmp);
                    e->push_back(tmp);
                 } catch  (CppOrmException e) {
                    char buf[20];
                    snprintf(buf, 19, "[%d]", i);
                    throw CppOrmException(e, buf);
                 }
            }
    }
    template<typename T>
    void get(const Json::Value& json, std::map<std::string, T>* e){
            if(!json.isObject()) {
                throw CppOrmException("shoulbe be a object");
            }
            Json::Value::Members keys = json.getMemberNames();
            for(Json::Value::Members::iterator it = keys.begin(); it != keys.end(); it++) {
                try { 
                    T tmp;
                    get(json[*it], &tmp);
                    (*e)[*it] = tmp;
                } catch  (CppOrmException e) {
                   throw CppOrmException(e, std::string(".") + *it);
                }
            }
    }
private: // for class type
    template<typename T>
    void get(const Json::Value& json, T* e){
       Mapper mapper(json, false);
       e->setORM(mapper);
    } 
private:  //for basic type
    void get(const Json::Value& json, int*);
    void get(const Json::Value& json, std::string *);

private:
    Json::Value json;
    bool is_encode;
};

}
