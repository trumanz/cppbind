#ifndef __CPPBIND__JSON_ENCODE_IMP__
#define __CPPBIND__JSON_ENCODE_IMP__

namespace  cppbind {

class EncodeMapper {
public:
    EncodeMapper(){
    }
    Json::Value getJson(){
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
       Mapper mapper(true, Json::Value());
       e.setBind(mapper);
       return mapper.getJson();
    } 
private:  //for basic type
    Json::Value encode( bool&);
    Json::Value encode( int32_t&);
    Json::Value encode( int64_t&);
    Json::Value encode( float&);
    Json::Value encode( double&);
    Json::Value encode( std::string &);
 private:
    Json::Value root;
};

 

}

#endif
