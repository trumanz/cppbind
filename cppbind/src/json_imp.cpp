#include "../include/cppbind_json_imp.hpp"
using namespace cppbind;

 Json::Value Mapper::getJson(){
        if(this->isEncode) {
            EncodeMapper* mapper = (EncodeMapper*)this->mapper_imp;
            return mapper->getJson();
        } else {
            DecodeMapper* mapper =  (DecodeMapper*)this->mapper_imp;
             return mapper->getJson();
        }
        
  }


 Mapper::Mapper(bool isEncode, Json::Value root){
        this->isEncode = isEncode;
        if(this->isEncode) {
           this->mapper_imp =  new EncodeMapper();
        } else {
           this->mapper_imp =  new DecodeMapper(root);
        }
}

Mapper::~Mapper(){
         if(this->isEncode) {
            EncodeMapper* mapper = (EncodeMapper*)this->mapper_imp;
            delete mapper;
        } else {
            DecodeMapper* mapper =  (DecodeMapper*)this->mapper_imp;
            delete mapper;
        }
}

void DecodeMapper::decode(const Json::Value& json, bool* e){
      e[0] =  json.asBool();
}
void DecodeMapper::decode(const Json::Value& json, int32_t* e){
      e[0] =  json.asInt();
}
void DecodeMapper::decode(const Json::Value& json, int64_t* e){
      e[0] =  json.asInt64();
}
void DecodeMapper::decode(const Json::Value& json, float* e){
      e[0] =  json.asFloat();
} 
void DecodeMapper::decode(const Json::Value& json, double* e){
      e[0] =  json.asDouble();
} 
void DecodeMapper::decode(const Json::Value& json, std::string* e){
      e[0] =  json.asString();
}

Json::Value EncodeMapper::encode(  bool& e){
    return Json::Value(e);
}
Json::Value EncodeMapper::encode(  int32_t& e){
    Json::Int x = e;
    return Json::Value(x);
}
Json::Value EncodeMapper::encode(  int64_t& e){
    Json::Int64 x = e;
    return Json::Value(x);
}

Json::Value EncodeMapper::encode(  float& e){
    return Json::Value(e);
}

Json::Value EncodeMapper::encode(  double& e){
    return Json::Value(e);
}

Json::Value EncodeMapper::encode(  std::string& e){
    return Json::Value(e);
}

