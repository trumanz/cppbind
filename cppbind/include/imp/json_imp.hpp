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


class CppBindException : public std::runtime_error
{
   public:
    CppBindException(std::string const& node_name, const std::string base_msg)
        : runtime_error(node_name + " "  + base_msg)
    { }
    
    CppBindException(const std::string base_msg)
        : runtime_error(std::string(" ")  + base_msg)
    { }
    CppBindException(const CppBindException& e, std::string const& node_name)
        : runtime_error(node_name + e.what())

    { }
    
};

class Binder{
public:
    Binder(bool isEncode, Json::Value root);
    virtual ~Binder();
    template<typename T>
    void bind(const std::string& name, T& v, const char *default_value = NULL);
    Json::Value getJson();
    void setJson(const Json::Value jv);
public:
    bool isEncode;
    void* binder_imp;
    //DecodeBinder decode_binder;
    //EncodeBinder encode_binder;
};

}

#include "imp/decode_binder.hpp"
#include "imp/encode_binder.hpp"


namespace cppbind {

template<typename T>
void Binder::bind(const std::string& name, T& v, const char *default_value){
        if(this->isEncode) {
            EncodeBinder* binder = (EncodeBinder*)this->binder_imp;
            binder->bind(name,v, default_value);
        } else {
            DecodeBinder* binder = (DecodeBinder*)this->binder_imp;
            binder->bind(name,v, default_value);
        }
}


}

#endif
