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
#include "cppbind_json_spec_type_base.hpp"

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

}

#include "imp/json_decode_imp.hpp"
#include "imp/json_encode_imp.hpp"


namespace cppbind {

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
