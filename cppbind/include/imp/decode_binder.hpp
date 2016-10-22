#ifndef __CPPBIND__JSONDECODE_IMP__
#define __CPPBIND__JSONDECODE_IMP__

namespace  cppbind {


class DecodeBinder : public JsonBinderBase {
public:
    DecodeBinder(Json::Value json){
        this->json = json;
     }
    const Json::Value& getJson() const{
        return this->json;
    }
    void setJson(const Json::Value& jv){
        this->json = jv;
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

private: //for std container type
    template<typename T>
    void decode(const Json::Value& json, std::vector<T>* e){
         std::list<T> v;
         decode(json, &v);
         e->insert(e->begin(), v.begin(), v.end());
    }
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
private: // for class type
    template<typename T>
    void decode(const Json::Value& json, T* e){
         Binder binder(json);
         e->setBind(&binder);
    } 
private:  //for basic type
    void decode(const Json::Value& json, bool*e){
         e[0] =  json.asBool();
    }
    void decode(const Json::Value& json, int32_t *e){
         e[0] =  json.asInt();
    }
    void decode(const Json::Value& json, int64_t *e){
         e[0] =  json.asInt64();
    }
    void decode(const Json::Value& json, float *e){
        e[0] =  json.asFloat();
    }
    void decode(const Json::Value& json, double *e){
        e[0] =  json.asDouble();
    }
    void decode(const Json::Value& json, std::string *e){
        e[0] =  json.asString();
    }
 private:
    Json::Value json;
};

}

#endif
