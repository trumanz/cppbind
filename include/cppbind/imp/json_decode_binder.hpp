#ifndef __JSON_DECODE_BINDER_H__
#define __JSON_DECODE_BINDER_H__

namespace  cppbind {


class JsonDecodeBinder : public BinderImpBase {
private:
    bool basic_wrapper_as_string;
public:
    JsonDecodeBinder(Json::Value json, bool basic_wrapper_as_string){
        this->json = json;
        this->basic_wrapper_as_string = basic_wrapper_as_string;
     }
    void setJson(const Json::Value& jv){
        this->json = jv;
    }
    template<typename T>
    void decode(T* e){
        this->decode(this->json, e);
    }
    template<typename T>
    void bind(const std::string& name, T& v){
         //printf("filed %s\n", name.c_str());
         Json::Value jv = json[name];
         if(!jv.isNull()) {
             try {
                decode(jv, &v);
             } catch (CppBindException e) {
                throw CppBindException(e, std::string(".") + name);
             }
         } else  {
              throw CppBindException(std::string(".") + name, "not found");
         }
    }

    template<typename T>
    void bind(const std::string& name, boost::shared_ptr<T>& v){
         Json::Value jv = json[name];
         if(!jv.isNull()) {
             T  e;
             bind(name, e);
             v = boost::shared_ptr<T>(new T(e));
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
    //list
    template<typename T>
    void decode(const Json::Value& json, std::list<T>* e){
            if(!json.isArray()) {
                throw CppBindException("should be a list");
            }
            for(int i = 0; i  < json.size(); i++) {
                 try { 
                    T tmp;
                    decode(json[i], &tmp);
                    e->push_back(tmp);
                 } catch  (CppBindException e) {
                    char buf[20];
                    sprintf(buf, "[%d]", i);
                    throw CppBindException(e, buf);
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
private: // for class type


    class MemberFunctionCaller{
    public:
        template<typename T>
        void call(T*e, Binder* binder){
            e->setBind(binder);
        }
    };

    class RunTimeBinderCaller{
    public:
        template<typename T>
        void call(T*e, Binder* binder){
            JsonDecodeBinder* json_decode_binder = dynamic_cast<JsonDecodeBinder*>(binder->binder_imp.get());
            Json::Value jv = json_decode_binder->json;
            if(!jv.isString()) {
                assert("bug" == NULL);
            }
            std::string str = jv.asString();
            e[0] = binder->str_convert_mgmt.fromString<T>(str);
         }

    };

    template<typename T>
    void decode(const Json::Value& json, T* e){
         Binder binder(boost::shared_ptr<BinderImpBase>(new JsonDecodeBinder(json, this->basic_wrapper_as_string)));

        
          typedef typename boost::mpl::if_c<has_member_function_setBind<void (T::*) (Binder*)>::value, 
           MemberFunctionCaller, RunTimeBinderCaller>::type CallerT;

          CallerT().call(e, &binder);

         //CallerT().call(e, &binder);
         //e->setBind(&binder);
    } 
private:  //for Json type
    void decode(const Json::Value& json, Json::Value *e){
        e[0] =  json;
    }
private:  //for basic type
    void decode(const Json::Value& json, bool*e){
        if(basic_wrapper_as_string) {
           JsonStrToBasicType(json,e);
           return;
        }
        e[0] =  json.asBool();
    }
    void decode(const Json::Value& json, int32_t *e){
         if(basic_wrapper_as_string) {
           JsonStrToBasicType(json,e);
           return;
         }
         e[0] =  json.asInt();   
    }
    void decode(const Json::Value& json, int64_t *e){
         if(basic_wrapper_as_string) {
           JsonStrToBasicType(json,e);
           return;
         }
         e[0] =  json.asInt64();
    }
    void decode(const Json::Value& json, float *e){
        if(basic_wrapper_as_string) {
           JsonStrToBasicType(json,e);
           return;
         }
        e[0] =  json.asFloat();
    }
    void decode(const Json::Value& json, double *e){
        if(basic_wrapper_as_string) {
           JsonStrToBasicType(json,e);
           return;
         }
        e[0] =  json.asDouble();
    }
    void decode(const Json::Value& json, std::string *e){
        if(basic_wrapper_as_string) {
           JsonStrToBasicType(json,e);
           return;
         }
        e[0] =  json.asString();
    }
    template<typename T>
    void  JsonStrToBasicType(const Json::Value& json, T*e){
        std::stringstream ss;
        ss << json.asString();
        ss >>  *e;
    }
    void  JsonStrToBasicType(const Json::Value& json, bool*e){
        if (json.asString() == "TRUE" || json.asString() == "true") {
            *e = true;
        } else {
            *e = false;
        }
    }
public:
    Json::Value json;
};

}

#endif
