#ifndef __JSON_DECODE_BINDER_H__
#define __JSON_DECODE_BINDER_H__

#include "cppbind/imp/decoder/fundamental_decode.hpp"
namespace  cppbind {


class JsonDecoderImp : public EnconderDecoderBase {
public:
    //Binder binder;
    bool ignore_unknown_key;
public:
    JsonDecoderImp(){ ignore_unknown_key = false; }
    void ignoreUnknownNode(){ ignore_unknown_key = true;}

    template<typename T>
    void bindForeginKey(const Json::Value& _jv, const std::string& name, T& v, const T* default_value);
    //Dynamic type
    template<typename T>
    void decodeDynamicType(const Json::Value& _jv, const std::string& name, T*& v, Json::Value* default_value );
    template<typename T>
    void decodeDynamicType(const Json::Value& _jv, const std::string& name, boost::shared_ptr<T> &v, boost::shared_ptr<T> default_value);
    template<typename T>
    void decodeDynamicType(const Json::Value &_jv, const std::string& name, std::vector<T*>& v, Json::Value* default_value);
private:
    template<typename T>
    void decodeDynamicType(const Json::Value& jv, T*& v);

public:
    //Fundamental Type
    void decode(const Json::Value& json, bool*e);
    void decode(const Json::Value& json, int32_t *e);
    void decode(const Json::Value& json, uint32_t *e);
    void decode(const Json::Value& json, int64_t *e);
    void decode(const Json::Value& json, uint64_t *e);
    void decode(const Json::Value& json, float *e);
    void decode(const Json::Value& json, double *e);
    void decode(const Json::Value& json, std::string *e);
    //stl container type
    template<typename T>
    void decode(const Json::Value& json, std::vector<T>* e);
    template<typename T>
    void decode(const Json::Value& json, std::vector<T*>* e);
    template<typename T>
    void decodeWithForeginKey(const Json::Value& json, std::vector<T*>* e);
    template<typename T>
    void decode(const Json::Value& json, std::list<T>* e);
    template<typename T>
    void decode(const Json::Value& json, std::list<T*>* e);
    template<typename T>
    void decode(const Json::Value& json, std::set<T>* e);
    template<typename KeyT, typename ValueT>
    void decode(const Json::Value& json, std::map<KeyT, ValueT>* e);

private:
    //call Calss:setBind
    class SetBindMemberFunctionCaller{
    public:
        template<typename T>
        void call(const Json::Value& jv, T*e, JsonDecoderImp* jbinder){
            Binder next_binder(jbinder, (Json::Value*)&jv);
            //std::cout << __FILE__ << __LINE__  <<  jv  << typeid(T).name() << "\n";
            e->setBind(&next_binder, true);
            if(!jbinder->ignore_unknown_key  && jv.isObject()) {
                Json::Value::Members keys = jv.getMemberNames();
                for(Json::Value::Members::iterator it = keys.begin(); it != keys.end(); it++) {
                    std::string key_str = *it;
                    if(next_binder.decoded_member_key_set.find(key_str) == next_binder.decoded_member_key_set.end()) {
                        auto e =  ParseErrorException(std::string("json value not mapping, please check json key:(")
                                + key_str + ") within setBind Function of class " + typeid(T).name());
                        Util::throwExeption(e);
                    }
                }

            }
        }
    };
    /*
    //call Calss:fromJsonValue4Bind
    class FromJsonValue4BindMemberFunctionCaller{
    public:
        template<typename T>
        void call(const Json::Value& jv, T*e, JsonDecoderImp* binder){
           e->fromJsonValue4Bind(jv);
        }
    };
     */
    //call str_convert_mgmt; TODO JsonMapManager
    class RunTimeBinderCaller{
    public:
        template<typename T>
        void call(const Json::Value& jv, T*e, JsonDecoderImp* binder){
            if(!jv.isString()) {
                std::stringstream ss; ss<< jv;
                printf("error: jv=%s", ss.str().c_str());
                assert("bug" == NULL);
            }
            e[0] = binder->str_convert_mgmt.fromString<T>(jv.asString());
         }

    };
    /*
    class Option2MemberFunctionCaller{
    public:
        template<typename T>
        void call(const Json::Value& jv, T*e, JsonDecoderImp* binder){
          //if have "void fromStr4Bind(const Json::Value&)" then call setbind; else call otheres
          typedef typename boost::mpl::if_c<
                  has_member_function_fromJsonValue4Bind<void (T::*) (const Json::Value&)>::value,
                  FromJsonValue4BindMemberFunctionCaller,
                  RunTimeBinderCaller>::type CallerT;

          CallerT().call(jv, e, binder); 
        }
    };
     */
public:
    //class/struct object type
    template<typename T>
    void decode(const Json::Value& json, T* e){
         //if have "void setBind(Binder*)" then call setbind; else call otheres
          typedef typename boost::mpl::if_c<
                  has_member_function_setBind<void (T::*) (Binder*, bool)>::value,
                  SetBindMemberFunctionCaller,
                  RunTimeBinderCaller >::type CallerT;
          CallerT().call(json, e, this);
    }
    template<typename AnyOtherT, typename T>
    void decode(const AnyOtherT x, T* e); //use template to avoid Json::Value implicit contruction

public:
    template<typename T>
    void decode(const Json::Value& json, T** e){
         *e  = new T();
         decode(json, *e);
    } 

    template<typename T>
    void decodeWithForeginKey(const Json::Value& json, T** e){
         std::string key;
         decode(json, &key);
         e[0] = this->getForeignObj<T>(key);
    } 

public:
    //Json type
    void decode(const Json::Value& json, Json::Value *e){ e[0] =  json; }

};

#include "cppbind/imp/decoder/JsonDecoder_funtamental_type_imp.h"
#include "cppbind/imp/decoder/JsonDecoder_stl_container_imp.h"
#include "cppbind/imp/decoder/JsonDecoder_dynamic_imp.h"

template<typename T>
void JsonDecoderImp::bindForeginKey(const Json::Value& _jv, const std::string& name, T& v, const T* default_value){
     //printf("filed %s\n", name.c_str());
     if(_jv.isMember(name)) {
         const Json::Value& jv = _jv[name];
         try {
            decodeWithForeginKey(jv, &v);
         } catch (ParseErrorException& e) {
             e.addParentNodeName(name);
             throw e;
         }
     } else if(default_value) {
         v = *default_value;

     }else  {
          throw ParseErrorException(name, "not found");
     }
}

}

#endif
