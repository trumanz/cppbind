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
#include <jsoncpp/json/writer.h>
namespace  cppbind {

template<typename T>
class JsonBind{
public:
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
         Binder binder(false, root);
         e->setBind(binder);
         return boost::shared_ptr<T>(e);
    }
     
    void encode(std::ostream *out, T &e){
         //EncodeBinder binder;
         Binder binder(true, Json::Value());
         e.setBind(binder);
         Json::StyledStreamWriter writer;
         writer.write(*out, binder.getJson());
    }

};


}

#endif
