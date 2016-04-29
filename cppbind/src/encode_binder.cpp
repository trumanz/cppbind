#include "../include/imp/json_imp.hpp"
using namespace cppbind;

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

