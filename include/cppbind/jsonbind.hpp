﻿#ifndef __CPPBIND__JSONBIND__
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
    template<typename ObjT>
    void regTable(const std::map<std::string, ObjT*> *table) { decoder.regTable(table);} 
    template<typename ObjT, typename FTable>
    void regTable(boost::shared_ptr<FTable> ft){ decoder.regTable<ObjT, FTable>(ft);}
    void regClassRegister(ClassRegisterBase* _class_reg){
        encoder.regClassRegister(_class_reg);
        decoder.regClassRegister(_class_reg);
    }
    void IgnoreUnknownKey(){ decoder.IgnoreUnknownKey();}
public:
    //decode API; v2
    template<typename T>
    T* decodeJsonString(const std::string& jv);
public:
    template<typename T>
    T* decode(std::istream &is){
         Json::Value root = decodeIStream2JsonValue(is);
         return decode<T>(root);
    }
    template<typename T>
    T* decode(const Json::Value& root){
         T* e = new T;
         decoder.DecodeJson((Json::Value*)&root, e);
         return e;
    }
    template<typename T>
    T* decodeFile(const char * file_name){
        Json::Value root = decodeFile2JsonValue(file_name);
        try{
            return decode<T>(root);
        }catch (cppbind::ParseErrorException& e){
            e.addParentNodeName(file_name);
            throw e;
        }
        return NULL;
    }

    Json::Value decodeIStream2JsonValue(std::istream &is){
         Json::Value root;
         Json::Reader reader;
         bool parsingSuccessful = reader.parse(is, root);
         if(!parsingSuccessful) {
			 std::string err_msg =  reader.getFormattedErrorMessages();
           printf("Failed to parse, %s\n", err_msg.c_str());
           throw  ParseErrorException(err_msg);
         }
         return root;
    }
    Json::Value decodeFile2JsonValue(std::string file_name) {
        Json::Value jv;
        try{
            std::fstream fs(file_name.c_str(), std::fstream::in);
            if(!fs) {
                throw ParseErrorException("Can not open file [" + file_name + "]");
            }
            jv =  this->decodeIStream2JsonValue(fs);
        }catch (ParseErrorException& e){
            e.addParentNodeName(file_name);
            throw e;
        } catch (std::runtime_error &e) {
            throw cppbind::ParseErrorException(file_name, e.what());
        }
        return jv;
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



template<typename T>
T* JsonBind::decodeJsonString(const std::string& jv)
{
    std::stringstream ss;
    ss <<jv;
    return this->decode<T>(ss);
}

}

#endif
