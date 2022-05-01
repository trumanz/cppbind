#ifndef __JSON_DECODE_BINDER_H__
#define __JSON_DECODE_BINDER_H__

#include "cppbind/imp/fundamental_decode.hpp"
namespace  cppbind {


class JsonDecoder : public BinderImpBase {
public:
    //Binder binder;
    bool ignore_unknown_key;
public:
    JsonDecoder(){ ignore_unknown_key = false; }
    void ignoreUnknownNode(){ ignore_unknown_key = true;}

    template<typename T>
    void bindForeginKey(const Json::Value& _jv, const std::string& name, T& v, const T* default_value);

    template<typename T>
    void bindDynamicType(const Json::Value& _jv, const std::string& name, T*& v,  Json::Value* default_value ){
        Json::Value jv = _jv[name];
        if(jv.isNull() && default_value != NULL) {
            jv = *default_value;
        }
        bindDynamicType(jv,v);
    }

    
    template<typename T>
    void bindDynamicType(const Json::Value& _jv, const std::string& name, boost::shared_ptr<T> &v, boost::shared_ptr<T> default_value){
        Json::Value jv = _jv[name];
        if(jv.isNull()) {
            v = default_value;
        } else {
            T* tmp_v = NULL;
            bindDynamicType(jv,tmp_v);
            v = boost::shared_ptr<T>(tmp_v);
        }
    }

private:
    template<typename T>
    void bindDynamicType(const Json::Value& jv, T*& v){
        Json::Value::Members  members = jv.getMemberNames();
        if(members.size() != 1) {
            throw ParseErrorException("DynamicType object must have a unique key as it's type");
        }
        std::string class_name = members[0];
        Json::Value class_data = jv[class_name];
       
        if(this->class_reg == NULL) {
            throw ParseErrorException("no class register");
        }
        v = this->class_reg->createObj<T>(class_name.c_str(), class_data, this);
    }
public:
    template<typename T>
    void bindDynamicType(const Json::Value &_jv, const std::string& name, std::vector<T*>& v, Json::Value* default_value){
        Json::Value jv = _jv[name];
        if(jv.isNull() && default_value != NULL) {
            jv = *default_value;
        }
        if(!jv.isArray()) {
            throw ParseErrorException(name, "should be a object array");
        }
        for(Json::Value::ArrayIndex i = 0; i  < jv.size(); i++) {
            T* e;
            if(!jv[i].isObject()) {
                throw ParseErrorException(name, "should be a object");
            }
            try {
                bindDynamicType(jv[i],e);
                v.push_back(e);
            } catch (ClassMissRegException& e) {
                if (!this->ignore_unknown_key) {
                    throw e;
                }
            } catch(ParseErrorException& e) {
                std::stringstream ss; ss << i;
                e.addParentNodeName(ss.str());
                throw e;
            }
        }
    }

//std container type
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
    //stl type
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

public: 
    //call Calss:setBind
    class SetBindMemberFunctionCaller{
    public:
        template<typename T>
        void call(const Json::Value& jv, T*e, JsonDecoder* jbinder){
            Binder next_binder(jbinder, (Json::Value*)&jv);

            //std::cout << __FILE__ << __LINE__  <<  jv  << typeid(T).name() << "\n";
            e->setBind(&next_binder, true);

            if(!jbinder->ignore_unknown_key  && jv.isObject()) {
                Json::Value::Members keys = jv.getMemberNames();
                for(Json::Value::Members::iterator it = keys.begin(); it != keys.end(); it++) {
                    std::string key_str = *it;
                    if(next_binder.decoded_member_key_set.find(key_str) == next_binder.decoded_member_key_set.end()) {
                        throw ParseErrorException(std::string("unknown json key:[") + key_str + "]");
                    }
                }

            }
        }
    };

    //call Calss:fromJsonValue4Bind
    class FromJsonValue4BindMemberFunctionCaller{
    public:
        template<typename T>
        void call(const Json::Value& jv, T*e, JsonDecoder* binder){
           e->fromJsonValue4Bind(jv);
        }
    };

    //call str_convert_mgmt; TODO JsonMapManager
    class RunTimeBinderCaller{
    public:
        template<typename T>
        void call(const Json::Value& jv, T*e, JsonDecoder* binder){
            if(!jv.isString()) {
                std::stringstream ss; ss<< jv;
                printf("error: jv=%s", ss.str().c_str());
                assert("bug" == NULL);
            }
            e[0] = binder->str_convert_mgmt.fromString<T>(jv.asString());
         }

    };


    class Option2MemberFunctionCaller{
    public:
        template<typename T>
        void call(const Json::Value& jv, T*e, JsonDecoder* binder){
          //if have "void fromStr4Bind(const Json::Value&)" then call setbind; else call otheres
          typedef typename boost::mpl::if_c<has_member_function_fromJsonValue4Bind<void (T::*) (const Json::Value&)>::value, 
              FromJsonValue4BindMemberFunctionCaller, RunTimeBinderCaller>::type CallerT;

          CallerT().call(jv, e, binder); 
        }
    };


    template<typename T>
    void decode(const Json::Value& json, T* e){

         //if have "void setBind(Binder*)" then call setbind; else call otheres
          typedef typename boost::mpl::if_c<has_member_function_setBind<void (T::*) (Binder*, bool)>::value, 
           SetBindMemberFunctionCaller, Option2MemberFunctionCaller>::type CallerT;

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

#include "JsonDecoder_funtamental_type_imp.h"
#include "JsonDecoder_stl_container_imp.h"

template<typename T>
void JsonDecoder::bindForeginKey(const Json::Value& _jv, const std::string& name, T& v, const T* default_value){
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
