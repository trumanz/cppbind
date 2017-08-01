#ifndef __CPPBIND__JSONBIND__
#define __CPPBIND__JSONBIND__

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <assert.h>
#include <stdexcept>

#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/json.h>
#include <boost/tti/has_member_function.hpp>
#include <boost/mpl/if.hpp>
#include <boost/shared_ptr.hpp>

#include "imp/binder.hpp"

namespace  cppbind {
class JsonBind{
public:

    template<typename T>
    boost::shared_ptr<T> decode(std::istream &is){
         Json::Value root;
         Json::Reader reader;
         bool parsingSuccessful = reader.parse(is, root);
         if(!parsingSuccessful) {
           printf("Failed to parse, %s\n", reader.getFormatedErrorMessages().c_str());
           throw  CppBindException(reader.getFormatedErrorMessages());
         }
         return this->decode<T>(root);
    }

    template<typename T>
    boost::shared_ptr<T> decode(const Json::Value& root){
         T* e = NULL;
         e = new T;
         JsonDecodeBinder decoder(root,false);
         decoder.decode(e);
         return boost::shared_ptr<T>(e);
    }

    template<typename T>
    T* decode2(const Json::Value& root, bool basic_wrapper_string){
         T* e = NULL;
         e = new T;
         JsonDecodeBinder decoder(root, basic_wrapper_string);
         decoder.decode(e);
         return e;
    }
     
    template<typename T>
    void encode(T&e, std::ostream *out){
         //EncodeBinder binder;
         JsonEncodeBinder encoder;
         bool dummy;
         Json::Value jv = encoder.encode(e, &dummy);
         Json::StyledStreamWriter writer;
         writer.write(*out, jv);
    }
    template<typename T>
    std::string toJsonStr(T&e){
         std::stringstream ss;
         encode(e, &ss);
         return ss.str();
    }

};


}

#endif
