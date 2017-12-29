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
#include "ClassRegisterBase.h"
#include "imp/binder.hpp"

namespace  cppbind {
class JsonBind{

public:
JsonEncodeBinder encoder;
JsonDecodeBinder decoder;
public:
    JsonBind(){
    }
    template<typename T>
    void regTable(const std::map<std::string, T*> *table)
    {
        encoder.regTable(table);
        decoder.regTable(table);
    }
    void regClassRegister(ClassRegisterBase* _class_reg){
        encoder.regClassRegister(_class_reg);
        decoder.regClassRegister(_class_reg);
    }
public:
    //decode API, !!!
    template<typename T>
    T* decodeJV2Point(const Json::Value& root, bool basic_wrapper_string = false){
         T* e = NULL;
         e = new T;
         decoder.DecodeJson((Json::Value*)&root, e);
         return e;
    }

    template<typename T>
    T* decodeIStream2Point(std::istream &is, bool basic_wrapper_string = false){
         Json::Value root;
         Json::Reader reader;
         bool parsingSuccessful = reader.parse(is, root);
         if(!parsingSuccessful) {
           printf("Failed to parse, %s\n", reader.getFormatedErrorMessages().c_str());
           throw  CppBindException(reader.getFormatedErrorMessages());
         }
         return decodeJV2Point<T>(root, basic_wrapper_string);
    }


    template<typename T>
    boost::shared_ptr<T> decode(std::istream &is){
        T* e = this->decodeIStream2Point<T>(is);
        return boost::shared_ptr<T>(e);
    }

    template<typename T>
    boost::shared_ptr<T> decode(const Json::Value& root){
         T* e = this->decodeJV2Point<T>(root);
         return boost::shared_ptr<T>(e);
    }

    template<typename T>
    T* decode2(const Json::Value& root, bool basic_wrapper_string){
        return this->decodeJV2Point<T>(root,basic_wrapper_string);
    }

    template<typename T>
    T* decodeFile(const char * file_name){
        std::fstream fs (file_name, std::fstream::in);
        if(!fs) {
            printf("ERROR Can not open file %s\n", file_name);
            assert("TODO throw exception" == NULL);
        }
        return this->decodeIStream2Point<T>(fs);
    }

//Encode Interface
public:
    //to JsonValue; !!!
    template<typename T>
    void  encodeToJsonValue(T&e, Json::Value* jv){
         encoder.encodeToJson(&e, jv);
    }
    template<typename T>
    Json::Value  encodeToJsonValue(T&e){
         Json::Value jv;
         this->encodeToJsonValue(e,&jv);
         return jv;
    }
    //to ostream
    template<typename T>
    void encode(T&e, std::ostream *out){
        Json::Value jv;
        this->encodeToJsonValue(e,&jv);
        Json::StyledStreamWriter writer;//TODO, performance ?
        writer.write(*out, jv);
    }

    //to string
    template<typename T>
    std::string toJsonStr(T&e){
         std::stringstream ss;
         this->encode(e, &ss);
         return ss.str();
    }


};


}

#endif
