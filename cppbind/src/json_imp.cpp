#include "../include/imp/json_imp.hpp"
using namespace cppbind;

 Json::Value Binder::getJson(){
        if(this->isEncode) {
            EncodeBinder* binder = (EncodeBinder*)this->binder_imp;
            return binder->getJson();
        } else {
            DecodeBinder* binder =  (DecodeBinder*)this->binder_imp;
             return binder->getJson();
        }
        
  }
void Binder::setJson(const Json::Value jv){
     if(this->isEncode) {
            EncodeBinder* binder = (EncodeBinder*)this->binder_imp;
             binder->setJson(jv);
        } else {
            DecodeBinder* binder =  (DecodeBinder*)this->binder_imp;
              binder->setJson(jv);
        }
}

 Binder::Binder(bool isEncode, Json::Value root){
        this->isEncode = isEncode;
        if(this->isEncode) {
           this->binder_imp =  new EncodeBinder();
        } else {
           this->binder_imp =  new DecodeBinder(root);
        }
}

Binder::~Binder(){
         if(this->isEncode) {
            EncodeBinder* binder = (EncodeBinder*)this->binder_imp;
            delete binder;
        } else {
            DecodeBinder* binder =  (DecodeBinder*)this->binder_imp;
            delete binder;
        }
}

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

/*
void DecodeBinder::decode(const Json::Value& json, SpecTypeBase * specValue){
     std::string errmsg;
     std::string str = json.toStyledString();
     int ret = specValue->decode(str, &errmsg);
     if(ret != 0){
        throw CppBindException(errmsg);
     }
}
*/
Json::Value EncodeBinder::encode(  bool& e){
    return Json::Value(e);
}
Json::Value EncodeBinder::encode(  int32_t& e){
    Json::Int x = e;
    return Json::Value(x);
}
Json::Value EncodeBinder::encode(  int64_t& e){
    Json::Int64 x = e;
    return Json::Value(x);
}

Json::Value EncodeBinder::encode(  float& e){
    return Json::Value(e);
}

Json::Value EncodeBinder::encode(  double& e){
    return Json::Value(e);
}

Json::Value EncodeBinder::encode(  std::string& e){
    return Json::Value(e);
}

