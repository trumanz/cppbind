#ifndef __CPPBIND__JsonEncoder__
#define __CPPBIND__JsonEncoder__

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

#include <json/writer.h>
#include <json/json.h>
#include <boost/tti/has_member_function.hpp>
#include <boost/mpl/if.hpp>
#include <boost/shared_ptr.hpp>
#include "cppbind/dynamic/ClassRegisterBase.h"
#include "imp/Binder.hpp"

namespace  cppbind {

 class JsonEncoder {

 public:
     JsonEncoderImp encoder;
 public:
   
     void regClassRegister(ClassRegisterBase *_class_reg) {
         encoder.regClassRegister(_class_reg);
     }

 public:
     template<typename T>
     Json::Value encode(T &e) {
         Json::Value jv;
         this->encode(e, &jv);
         return jv;
     }

     template<typename T>
     void encode(T &e, Json::Value *jv) {
         encoder.encodeToJson(&e, jv);
     }

     template<typename T>
     void encode(T &e, std::ostream *out) {
         Json::Value jv;
         this->encode(e, &jv);
         Json::StyledStreamWriter writer;//TODO, performance ?
         writer.write(*out, jv);
     }

     template<typename T>
     std::string encodeToStr(T &e) {
         std::stringstream ss;
         this->encode(e, &ss);
         return ss.str();
     }
 };
}
#endif
