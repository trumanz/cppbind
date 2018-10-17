#ifndef __JSON_DECODE_BINDER_H__
#define __JSON_DECODE_BINDER_H__

namespace  cppbind {


class JsonDecodeBinder : public BinderImpBase {
public:
    //Binder binder;
    bool ignore_unknown_key;
public:
    JsonDecodeBinder(){
        ignore_unknown_key = false;
       // binder.init(this, NULL);
    }
    void IgnoreUnknownKey(){
        ignore_unknown_key = true;
    }

    template<typename T>
    void DecodeJson(Json::Value* jv, T* e) {
        //binder.init(this, jv);
        this->decode(*jv, e);
    }    

//bind API
    template<typename T>
    void bind(const Json::Value& _jv, const std::string& name, T& v){

         const Json::Value& jv = _jv[name];
         //std::cout << __FILE__ << __LINE__  << ": " << name << "\n";
         if(!jv.isNull()) {
             try {
                decode(jv, &v);
                //std::cout << __FILE__ << __LINE__  << ": " << name << "\n";
             } catch (CppBindException e) {
                //std::cout << "error:" << __FILE__ << __LINE__  <<  name  <<","<<  typeid(T).name() << ","<< jv  << "\n";
                throw CppBindException(e, std::string(".") + name);
             }
         } else  {
              throw CppBindException(std::string(".") + name, std::string("not found"));
         }
    }
    template<typename T>
    void bind(const Json::Value& _jv, const std::string& name, T& v, const T& default_vaule){
        const Json::Value& jv = _jv[name];
        if(!jv.isNull()) {
            try {
               decode(jv, &v);
            } catch (CppBindException e) {
               throw CppBindException(e, std::string(".") + name);
            }
        } else  {
             v = default_vaule;
        }
    }
    template<typename T>
    void bind(const Json::Value& _jv, const std::string& name, boost::shared_ptr<T>& v){
        const Json::Value& jv = _jv[name];
         if(!jv.isNull()) {
             T  e;
             bind(_jv, name, e);
             v = boost::shared_ptr<T>(new T(e));
         } 
    }

    
    template<typename T>
    void bindForeginKey(const Json::Value& _jv, const std::string& name, T& v, const T* default_value);

    template<typename T>
    void bindWithDynamicType(const Json::Value& _jv, const std::string& name, T*& v,  Json::Value* default_value ){
        Json::Value jv = _jv[name];
        if(jv.isNull() && default_value != NULL) {
            jv = *default_value;
        }
        bindDynamicType(jv,v);
    }

    template<typename T>
    void bindDynamicType(const Json::Value& jv, T*& v){
        Json::Value::Members  members = jv.getMemberNames();
        if(members.size() != 1) {
            std::stringstream ss;
            ss << jv;
            printf("jv=%s", ss.str().c_str());
            assert(false);
        }
        std::string class_name = members[0];
        Json::Value class_data = jv[class_name];
        try{
            if(this->binder_data.class_reg == NULL) {
                printf("ERROR, please register classRegister first");
            }
            v = this->binder_data.class_reg->createObj<T>(class_name.c_str(), class_data, this);
        }catch (CppBindException& e) {
                throw CppBindException(e, std::string(".") + class_name);
        }
    }

    template<typename T>
    void bindDynamicTypeArray(const Json::Value &_jv, const std::string& name, std::vector<T*>& v){
        const Json::Value& jv = _jv[name];
        if(!jv.isArray()) {
            throw CppBindException(std::string(".") + name, "should be a obj array");
            assert(false);
        }
        for(Json::Value::ArrayIndex i = 0; i  < jv.size(); i++) {
            T* e;
            if(!jv[i].isObject()) {
                throw CppBindException(std::string(".") + name, "should be a obj array");
                assert(false);
            }
            try {
                bindDynamicType(jv[i],e);
                v.push_back(e);
            } catch (ClassMissRegException& e) {
                if (!this->ignore_unknown_key) {
                    printf("ERROR %s\n", e.what());
                    assert(false);
                }
            } catch(CppBindException& e) {
                std::stringstream ss; ss << i;
                throw CppBindException(e, std::string(".") + ss.str());
            }
        }
    }

//std container type
private: 
    //vector
    template<typename T>
    void decode(const Json::Value& json, std::vector<T>* e){
         std::list<T> v;
         decode(json, &v);
         e->insert(e->begin(), v.begin(), v.end());
    }

    //vector
    template<typename T>
    void decode(const Json::Value& json, std::vector<T*>* e){
         std::list<T*> v;
         decode(json, &v);
         e->insert(e->begin(), v.begin(), v.end());
    }
    template<typename T>
    void decodeWithForeginKey(const Json::Value& json, std::vector<T*>* e){
         std::vector<std::string> keys;
         if(!json.isArray()) {
                throw CppBindException("should be a list");
         }
		 for(Json::Value::ArrayIndex i = 0; i  < json.size(); i++) {
             T* x = NULL;
             decodeWithForeginKey(json[i], &x);
             e->push_back(x);
         }
    }
    //list
    template<typename T>
    void decode(const Json::Value& json, std::list<T>* e){
            if(!json.isArray()) {
                throw CppBindException("should be a list");
            }
            for(Json::Value::ArrayIndex i = 0; i  < json.size(); i++) {
                 try { 
                    T tmp;
                    decode(json[i], &tmp);
                    e->push_back(tmp);
                 } catch  (CppBindException e) {
					 std::stringstream ss;
					 ss << "[" << i << "]";
                    throw CppBindException(e, ss.str());
                 }
            }
    }
    //list
    template<typename T>
    void decode(const Json::Value& json, std::list<T*>* e){
            if(!json.isArray()) {
                throw CppBindException("should be a list");
            }
			for(Json::Value::ArrayIndex i = 0; i  < json.size(); i++) {
                 try { 
                    T*  tmp = new T();
                    decode(json[i], tmp);
                    e->push_back(tmp);
                 } catch  (CppBindException e) {
					 std::stringstream ss;
					 ss << "[" <<  i << "]";
                    throw CppBindException(e, ss.str());
                 }
            }
    }
    //set
    template<typename T>
    void decode(const Json::Value& json, std::set<T>* e){
         std::list<T> v;
         decode(json, &v);
         for(typename std::list<T>::iterator it = v.begin(); it != v.end(); it++) {
             e->insert(*it);
         }
    }
    #if 0
    //map
    template<typename T>
    void decode(const Json::Value& json, std::map<std::string, T>* e){
            if(!json.isObject()) {
                throw CppBindException("shoulbe be a object");
            }
            Json::Value::Members keys = json.getMemberNames();
            for(Json::Value::Members::iterator it = keys.begin(); it != keys.end(); it++) {
                try { 
                    T tmp;
                    decode(json[*it], &tmp);
                    (*e)[*it] = tmp;
                } catch  (CppBindException e) {
                   throw CppBindException(e, std::string(".") + *it);
                }
            }
    }
#endif

    //map
    template<typename KeyT, typename ValueT>
    void decode(const Json::Value& json, std::map<KeyT, ValueT>* e){

            if(!json.isObject()) {
                throw CppBindException("shoulbe be a object");
            }
            Json::Value::Members keys = json.getMemberNames();
            for(Json::Value::Members::iterator it = keys.begin(); it != keys.end(); it++) {
                try {
                    //decode key
                    KeyT key; 
                    std::string key_str = *it;
                    Json::Value key_jv(key_str);
                    decode(key_jv, &key);

                    //decode value
                    ValueT value;
                    Json::Value value_jv = json[*it];


                    decode(value_jv, &value);
                    (*e)[key] = value;
                } catch  (CppBindException e) {
                   throw CppBindException(e, std::string(".") + *it);
                }
            }
    }
#if 0
    //map point
    template<typename T>
    void decode(const Json::Value& json, std::map<std::string, T*>* e){
            if(!json.isObject()) {
                throw CppBindException("shoulbe be a object");
            }
            Json::Value::Members keys = json.getMemberNames();
            for(Json::Value::Members::iterator it = keys.begin(); it != keys.end(); it++) {
                try { 
                    T* tmp = new T();
                    decode(json[*it], tmp);
                    (*e)[*it] = tmp;
                } catch  (CppBindException e) {
                   throw CppBindException(e, std::string(".") + *it);
                }
            }
    }
#endif

//decode for class Type

public: 
    //call Calss:setBind
    class SetBindMemberFunctionCaller{
    public:
        template<typename T>
        void call(const Json::Value& jv, T*e, JsonDecodeBinder* jbinder){
            Binder next_binder;
            next_binder.init(jbinder, (Json::Value*)&jv);

            //std::cout << __FILE__ << __LINE__  <<  jv  << typeid(T).name() << "\n";
            e->setBind(&next_binder, true);

            if(!jbinder->ignore_unknown_key) {
                assert(jv.isObject());
                Json::Value::Members keys = jv.getMemberNames();
                for(Json::Value::Members::iterator it = keys.begin(); it != keys.end(); it++) {
                    std::string key_str = *it;
                    if(next_binder.decoded_member_key_set.find(key_str) == next_binder.decoded_member_key_set.end()) {
                        throw CppBindException(std::string("unknown json key:[") + key_str + "]");
                    }
                }

            }
        }
    };

    //call Calss:fromJsonValue4Bind
    class FromJsonValue4BindMemberFunctionCaller{
    public:
        template<typename T>
        void call(const Json::Value& jv, T*e, JsonDecodeBinder* binder){
           e->fromJsonValue4Bind(jv);
        }
    };

    //call str_convert_mgmt; TODO JsonMapManager
    class RunTimeBinderCaller{
    public:
        template<typename T>
        void call(const Json::Value& jv, T*e, JsonDecodeBinder* binder){
            if(!jv.isString()) {
                std::stringstream ss; ss<< jv;
                printf("error: jv=%s", ss.str().c_str());
                assert("bug" == NULL);
            }
            e[0] = binder->binder_data.str_convert_mgmt.fromString<T>(jv.asString());
         }

    };


    class Option2MemberFunctionCaller{
    public:
        template<typename T>
        void call(const Json::Value& jv, T*e, JsonDecodeBinder* binder){
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

private:  //for Json type
    void decode(const Json::Value& json, Json::Value *e){
        e[0] =  json;
    }
private:  //for basic type
    void decode(const Json::Value& json, bool*e){
        if(json.isString()) {
            this->JsonStrToBasicType(json,e);
            return;
        } 
        e[0] =  json.asBool();
    }
    void decode(const Json::Value& json, int32_t *e){
        if(json.isString()) {
            this->JsonStrToBasicType(json,e);
            return;
        } 
         e[0] =  json.asInt();   
    }
    void decode(const Json::Value& json, uint32_t *e){
        if(json.isString()) {
            this->JsonStrToBasicType(json,e);
            return;
        } 
         e[0] =  json.asUInt();   
    }
    void decode(const Json::Value& json, int64_t *e){
        if(json.isString()) {
            this->JsonStrToBasicType(json,e);
            return;
        } 
         e[0] =  json.asInt64();
    }
    void decode(const Json::Value& json, uint64_t *e){
        if(json.isString()) {
            this->JsonStrToBasicType(json,e);
            return;
        } 
         e[0] =  json.asUInt64();   
    }
    void decode(const Json::Value& json, float *e){
        if(json.isString()) {
            this->JsonStrToBasicType(json,e);
            return;
        } 
        e[0] =  json.asFloat();
    }
    void decode(const Json::Value& json, double *e){
        if(json.isString()) {
            this->JsonStrToBasicType(json,e);
            return;
        } 
        e[0] =  json.asDouble();
    }
    void decode(const Json::Value& json, std::string *e){
        e[0] =  json.asString();
    }
    template<typename T>
    void  JsonStrToBasicType(const Json::Value& json, T*e){
        std::stringstream ss;
        ss << json.asString();
        ss >>  *e; //TODO wrong 
    }
    void  JsonStrToBasicType(const Json::Value& json, bool*e){
        if (json.asString() == "TRUE" || json.asString() == "true") {
            *e = true;
        } else {
            *e = false;
        }
    }
};

template<typename T>
void JsonDecodeBinder::bindForeginKey(const Json::Value& _jv, const std::string& name, T& v, const T* default_value){
     //printf("filed %s\n", name.c_str());
     if(_jv.isMember(name)) {
         const Json::Value& jv = _jv[name];
         try {
            decodeWithForeginKey(jv, &v);
         } catch (CppBindException e) {
            throw CppBindException(e, std::string(".") + name);
         }
     } else if(default_value) {
         v = *default_value;

     }else  {
          throw CppBindException(std::string(".") + name, "not found");
     }
}

}

#endif
