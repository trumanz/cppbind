#include "../include/imp/json_imp.hpp"
using namespace cppbind;

void DecodeBinder::decode(const Json::Value& json, bool* e){
      e[0] =  json.asBool();
}
void DecodeBinder::decode(const Json::Value& json, int32_t* e){
      e[0] =  json.asInt();
}
void DecodeBinder::decode(const Json::Value& json, int64_t* e){
      e[0] =  json.asInt64();
}
void DecodeBinder::decode(const Json::Value& json, float* e){
      e[0] =  json.asFloat();
} 
void DecodeBinder::decode(const Json::Value& json, double* e){
      e[0] =  json.asDouble();
} 
void DecodeBinder::decode(const Json::Value& json, std::string* e){
      e[0] =  json.asString();
}

