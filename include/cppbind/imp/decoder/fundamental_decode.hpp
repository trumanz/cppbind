#pragma once

//https://en.cppreference.com/w/cpp/language/types

namespace  cppbind {  namespace imp {
class FundamentalType {
public:
  template<typename T>
  void  decode_fundamental(const Json::Value& json, T*e){
    std::stringstream ss;
    ss << json.asString();
    ss >>  *e; //TODO wrong
  }
  void  decode_fundamental(const Json::Value& json, bool*e){
    if (json.asString() == "TRUE" || json.asString() == "true") {
        *e = true;
    } else {
        *e = false;
    }
}
};
}}
