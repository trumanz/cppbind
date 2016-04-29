#ifndef __CPPBIND__JSONDECODE_IMP__
#define __CPPBIND__JSONDECODE_IMP__

namespace  cppbind {


class DecodeMapper {
public:
    DecodeMapper(Json::Value json){
        //printf("%s\n", json.asCString());
        this->json = json;
     }
    Json::Value getJson(){
        return this->json;
    }
    void setJson(const Json::Value& jv){
        this->json = jv;
    }
    template<typename T>
    void bind(const std::string& name, T& v, const char *default_value = NULL){
         //printf("filed %s\n", name.c_str());
         Json::Value jv = json[name];
         if(!jv.isNull()) {
             try {
                decode(jv, &v);
                //printf("name:%s , type: %s\n", name.c_str(), typeid(v).name());
                //SpecTypeBase *specValue = dynamic_cast<SpecTypeBase *>(*v);
                //if(specValue != NULL) {
                //    printf("could cast to SpecTypeBase\n");
               // }
             } catch (CppBindException e) {
                throw CppBindException(e, std::string(".") + name);
             }
         } else  {
              throw CppBindException(std::string(".") + name, "not found");
         }
    }

    template<typename T>
    void bind(const std::string& name, boost::shared_ptr<T>& v, const char *default_value = NULL){
        //printf("filed %s\n", name.c_str());
         Json::Value jv = json[name];
         if(!jv.isNull()) {
             T  e;
             bind(name, e);
             v = boost::shared_ptr<T>(new T(e));
         } 
    }

private: //for std container type
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
         Mapper mapper(false, json);
         e->setBind(mapper);
    } 
private:  //for basic type
    void decode(const Json::Value& json, bool*);
    void decode(const Json::Value& json, int32_t *);
    void decode(const Json::Value& json, int64_t *);
    void decode(const Json::Value& json, float *);
    void decode(const Json::Value& json, double *);
    void decode(const Json::Value& json, std::string *);
    //void decode(const Json::Value& json, SpecTypeBase *);
private:
    Json::Value json;
};

}

#endif
