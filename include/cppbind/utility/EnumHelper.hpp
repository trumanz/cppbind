#ifndef _CPPBIND__ENUM_HELPER__
#define _CPPBIND__ENUM_HELPER__


namespace  cppbind {


template<typename T>
class EnumHelper
{
public:
    typename T::Type  value;

    EnumHelper(){
    }
    EnumHelper(typename T::Type v){
        this->value = v;
    }
    Json::Value toJsonValue4Bind(){  
        return Json::Value(this->toStr4Bind());
    } 
    void fromJsonValue4Bind(const Json::Value& jv){
         assert(jv.isString());
         std::string str = jv.asString();
         this->fromStr4Bind(str);
   }
private:
    std::string toStr4Bind(){  
        int x = value;
        assert(x < T::enum_str_info_len);
        return T::enum_str_info[x];
    } 

   void fromStr4Bind(const std::string& str){
       int x = -1;
       for(size_t i = 0; i < T::enum_str_info_len; i++) {
           if(str == T::enum_str_info[i]) {
              x = (int)i;
              break;
           }
       }
       assert(x != -1);
       this->value = (typename T::Type)x;
   }
public:

   bool operator==(const typename T::Type& _enum) const{
       return this->value == _enum;
   }
   bool operator==(const EnumHelper& r) const{
       return this->value == r.value;
   }
   bool operator<(const EnumHelper& right)const{
       return this->value <  right.value;
   }
};

}

#endif
