#ifndef _CPPBIND__ENUM_HELPER__
#define _CPPBIND__ENUM_HELPER__


namespace  cppbind {


template<typename T>
class EnumHelper
{
public:
    typename T::Type  value;
    Json::Value toJsonValue4Bind(){  
        int x = value;
        assert(x < T::enum_str_info_len);
        return Json::Value(T::enum_str_info[x]);
    } 

    void fromJsonValue4Bind(const Json::Value& jv){
         assert(jv.isString());
         std::string str = jv.asString();
         int x = -1;
         for(size_t i = 0; i < T::enum_str_info_len; i++) {
             if(str == T::enum_str_info[i]) {
                x = i;
                break;
             }
         }
         assert(x != -1);
         this->value = (typename T::Type)x;
   }

   bool operator==(const typename T::Type& _enum){
       return this->value == _enum;
   }
};

}

#endif
