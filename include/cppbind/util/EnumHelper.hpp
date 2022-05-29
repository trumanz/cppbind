#ifndef _CPPBIND__ENUM_HELPER__
#define _CPPBIND__ENUM_HELPER__

#include <assert.h>
#include <string>
#include "json/json.h"

namespace  cppbind { namespace util {

template<typename T>
class EnumHelper {
 public:
  typename T::Type  value;

  EnumHelper()  = default;
  EnumHelper(typename T::Type v) { this->value = v;}

  void setBind(Binder *binder, bool load){
      if(load == true) {
          assert(binder->json->isString());
          std::string str = binder->json->asString();
          this->fromStr4Bind(str);
      } else {
          binder->json[0] = Json::Value(this->valueStr());
      }
  }
  std::string valueStr() const {
    int x = value;
    assert(x < static_cast<int>(T::enum_str_info_len));
    return T::enum_str_info[x];
  }

 private:
  void fromStr4Bind(const std::string& str) {
    int x = -1;
    for (size_t i = 0; i < T::enum_str_info_len; i++) {
      if (str == T::enum_str_info[i]) {
        x = static_cast<int>(i);
        break;
      }
    }
    assert(x != -1);
    this->value = (typename T::Type)x;
  }

 public:
  bool operator==(const typename T::Type& _enum) const {
    return this->value == _enum;
  }
  bool operator==(const EnumHelper& r) const {
    return this->value == r.value;
  }
  bool operator<(const EnumHelper& right) const {
    return this->value <  right.value;
  }
};

}}  // namespace cppbind::util

#endif
