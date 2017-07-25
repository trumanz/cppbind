#ifndef __CPPBIND__JSONBIND__
#define __CPPBIND__JSONBIND__

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
#include "imp/binder.hpp"
#include "imp/csvcpp.h"
#include <jsoncpp/json/writer.h>
namespace  cppbind {

class JsonBind{
public:
    template<typename T>
    boost::shared_ptr<T> decode(std::istream &is){
         T* e = NULL;
         Json::Value root;
         Json::Reader reader;
         bool parsingSuccessful = reader.parse(is, root);
   
         if(!parsingSuccessful) {
           printf("Failed to parse, %s\n", reader.getFormatedErrorMessages().c_str());
           throw  CppBindException(reader.getFormatedErrorMessages());
         }

         e = new T;
         //Binder binder(root);
         //e->setBind(&binder);
         JsonDecodeBinder decoder(root);
         decoder.decode(e);
         return boost::shared_ptr<T>(e);
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
