#ifndef __CPPBIND__JSON_ENCODE_IMP__
#define __CPPBIND__JSON_ENCODE_IMP__

namespace  cppbind {

class EncodeBinder : public JsonBinderBase {
public:
    EncodeBinder(){
    }
    const Json::Value& getJson() const {
        return this->root;
    }
    void setJson(const Json::Value& jv){
        this->root = jv;
    }
    template<typename T>
    void bind(const std::string& name, T& v, const char *default_value = NULL){
          Json::Value jv =  encode(v);
          root[name] = jv;
    }

    template<typename T>
    void bind(const std::string& name, boost::shared_ptr<T>& v, const char *default_value = NULL){
          if(v.get() != NULL) {
               Json::Value jv =  encode(*(v.get()));
               root[name] = jv;
          }
    }

private: //for std container type
    
    template<typename T>
    Json::Value encode(std::vector<T>& e){
        Json::Value jv;
        for(typename std::vector<T>::iterator  it  = e.begin(); it != e.end(); it++) {
             Json::Value je = encode(*it);
             jv.append(je);
        }
        return jv;
    }
    template<typename T>
    Json::Value encode(std::list<T>& e){
        Json::Value jv;
        for(typename std::list<T>::iterator  it  = e.begin(); it != e.end(); it++) {
             Json::Value je = encode(*it);
             jv.append(je);
        }
        return jv;
    }
    template<typename T>
    Json::Value encode(std::map<std::string, T>& e){
            Json::Value jv;
            for(typename std::map<std::string, T>::iterator it =  e.begin(); it != e.end(); it++) {
                Json::Value je = encode(it->second);
                jv[it->first] = je;
            }
            return jv;
    }
private: // for class type
    template<typename T>
    Json::Value encode(T& e){
       Binder binder;
       e.setBind(&binder);
       return binder.getJson();
    } 
private:  //for basic type
    Json::Value encode( bool&e) {return Json::Value(e);}
    Json::Value encode( int32_t&e) { Json::Int x = e; return Json::Value(x); }
    Json::Value encode( int64_t&e) {Json::Int64 x = e; return Json::Value(x); }
    Json::Value encode( float&e){  return Json::Value(e); }
    Json::Value encode( double&e) { return Json::Value(e); }
    Json::Value encode( std::string &e) { return Json::Value(e);}
 private:
    Json::Value root;
};

 

}

#endif
