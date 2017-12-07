#ifndef __JSON_DECODE_BINDER_H__
#define __JSON_DECODE_BINDER_H__

namespace  cppbind {


class JsonDecodeBinder : public BinderImpBase {
private:
    bool basic_wrapper_as_string;
    std::map<std::string, boost::any>* type_tables;
public:
    JsonDecodeBinder(Json::Value json, bool basic_wrapper_as_string, std::map<std::string, boost::any>* type_tables){
        this->json = json;
        this->basic_wrapper_as_string = basic_wrapper_as_string;
        this->type_tables = type_tables;
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
    void bind(const std::string& name, T& v, const T& default_vaule){
        Json::Value jv = json[name];
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
    void bind(const std::string& name, boost::shared_ptr<T>& v){
         Json::Value jv = json[name];
         if(!jv.isNull()) {
             T  e;
             bind(name, e);
             v = boost::shared_ptr<T>(new T(e));
         } 
    }

    
    template<typename T>
    void bindWithForeginKey(const std::string& name, T& v){
         //printf("filed %s\n", name.c_str());
         Json::Value jv = json[name];
         if(!jv.isNull()) {
             try {
                decodeWithForeginKey(jv, &v);
             } catch (CppBindException e) {
                throw CppBindException(e, std::string(".") + name);
             }
         } else  {
              throw CppBindException(std::string(".") + name, "not found");
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
         for(int i = 0; i  < json.size(); i++) {
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
    //list
    template<typename T>
    void decode(const Json::Value& json, std::list<T*>* e){
            if(!json.isArray()) {
                throw CppBindException("should be a list");
            }
            for(int i = 0; i  < json.size(); i++) {
                 try { 
                    T*  tmp = new T();
                    decode(json[i], tmp);
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
         Binder binder(boost::shared_ptr<BinderImpBase>(new JsonDecodeBinder(json, this->basic_wrapper_as_string, this->type_tables)));

        
          typedef typename boost::mpl::if_c<has_member_function_setBind<void (T::*) (Binder*)>::value, 
           MemberFunctionCaller, RunTimeBinderCaller>::type CallerT;

          CallerT().call(e, &binder);
    } 

    
    template<typename T>
    void decodeWithForeginKey(const Json::Value& json, T** e){
         std::string type_name = typeid(T).name();
         std::map<std::string, boost::any>::iterator it = type_tables->find(type_name);
         if(it == type_tables->end()) {
             printf("ERROR, Please register table for %s\n", type_name.c_str());
             assert(false); 
         }
         boost::any any_table = it->second;
         const std::map<std::string, T*>* table= boost::any_cast<const std::map<std::string, T*>*>(it->second);
         assert(table != NULL);
         std::string key;
         decode(json, &key);
         typename std::map<std::string, T*>::const_iterator it2 = table->find(key);
         if(it2 == table->end()) {
             printf("ERROR, Can not found %s in table for %s", key.c_str(), type_name.c_str());
             assert(false);
         }
         T* x = it2->second;
         e[0] = x;
        
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
