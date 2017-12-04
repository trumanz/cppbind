#ifndef __JSON_ENCODE_BINDER_H__
#define __JSON_ENCODE_BINDER_H__

namespace  cppbind {

class JsonEncodeBinder : public BinderImpBase {
public:
    JsonEncodeBinder(){
    }
    void setJson(const Json::Value& jv){
        this->root = jv;
    }
    template<typename T>
    void bind(const std::string& name, T& v){
          bool dummy;
          Json::Value jv =  encode(v, &dummy);
          root[name] = jv;
    }

    template<typename T>
    void bind(const std::string& name, T& v, const T& default_value){
          bool dummy;
          Json::Value jv =  encode(v, &dummy);
          root[name] = jv;
    }


    template<typename T>
    void bind(const std::string& name, boost::shared_ptr<T>& v){
          if(v.get() != NULL) {
               bool dummy;
               Json::Value jv =  encode(*(v.get()), &dummy);
               root[name] = jv;
          }
    }

    template<typename T>
    void bindWithForeginKey(const std::string& name, T& v){
          bool dummy;
          Json::Value jv =  encodeWithForeginKey(v, &dummy);
          root[name] = jv;
    }
    
public: //for std container type
    
    template<typename T>
    Json::Value encode(std::vector<T>& e,bool* is_basic_type){
        *is_basic_type = false;
        Json::Value jv;
        for(typename std::vector<T>::iterator  it  = e.begin(); it != e.end(); it++) {
            bool dummy;
             Json::Value je = encode(*it, &dummy);
             jv.append(je);
        }
        return jv;
    }
    template<typename T>
    Json::Value encode(std::vector<T*>& e,bool* is_basic_type){
        *is_basic_type = false;
        Json::Value jv;
        for(typename std::vector<T*>::iterator  it  = e.begin(); it != e.end(); it++) {
            bool dummy;
             Json::Value je = encode(*(*it), &dummy);
             jv.append(je);
        }
        return jv;
    }
    template<typename T>
    Json::Value encodeWithForeginKey(std::vector<T*>& e,bool* is_basic_type){
        *is_basic_type = false;
        Json::Value jv;
        for(typename std::vector<T*>::iterator  it  = e.begin(); it != e.end(); it++) {
             bool dummy;
             T* x = *it;
             std::string key = x->getKeyStr();
             Json::Value je = encode(key, &dummy);
             jv.append(je);
        }
        return jv;
    }
    template<typename T>
    Json::Value encode(std::list<T*>& e,bool* is_basic_type){
        *is_basic_type = false;
        Json::Value jv;
        for(typename std::list<T*>::iterator  it  = e.begin(); it != e.end(); it++) {
             T& e_x = *(*it);
             bool dummy;
             Json::Value je = encode(e_x, &dummy);
             jv.append(je);
        }
        return jv;
    }
    template<typename T>
    Json::Value encode(std::list<T>& e,bool* is_basic_type){
        *is_basic_type = false;
        Json::Value jv;
        for(typename std::list<T>::iterator  it  = e.begin(); it != e.end(); it++) {
             bool dummy;
             Json::Value je = encode(*it, &dummy);
             jv.append(je);
        }
        return jv;
    }
    template<typename T>
    Json::Value encode(std::set<T>& e,bool* is_basic_type){
        Json::Value jv;
        is_basic_type[0] = false;
        if(e.empty()) return Json::arrayValue;
        for(typename std::set<T>::iterator  it  = e.begin(); it != e.end(); it++) {
            bool dummy;
             Json::Value je = encode(*(T*)(&(*it)), &dummy);
             jv.append(je);
        }
        return jv;
    }

   
    template<typename KeyT, typename ValueT>
    Json::Value encode(std::map<KeyT, ValueT>& e,bool* is_basic_type){
        *is_basic_type = false;
        Json::Value jv;
        for(typename std::map<KeyT, ValueT>::iterator it =  e.begin(); it != e.end(); it++) {
            bool is_basic_type;
            bool dummy;
            const KeyT& k = it->first;
            Json::Value k_jv = encode(k, &is_basic_type);
            Json::Value v_jv = encode(it->second, &dummy);
            if(is_basic_type) {
                std::string x = k_jv.toStyledString();
                if(x[x.length()-1] == '\n') {
                    x = x.substr(0, x.length()-1);
                }
                 jv[x] = v_jv;
            } 
            else {
                Json::Value je;
                je["_key"] = k_jv;
                je["_value"] = v_jv;
                je.append(je);
            }
        }
        return jv;
    }
//CLASS&STRUCT type
public:
    
    class MemberFunctionCaller{
    public:
        template<typename T>
        void call(T&e, Binder* binder){
            e.setBind(binder);
        }
    };

    class RunTimeBinderCaller{
    public:
        template<typename T>
        void call(T&e, Binder* binder){
            std::string x = binder->str_convert_mgmt.toString(e);
            JsonEncodeBinder* json_encoder_binder = dynamic_cast<JsonEncodeBinder*>(binder->binder_imp.get());
            json_encoder_binder->setJson(Json::Value(x));
        }
    };
    
     
    template<typename T>
    Json::Value encode(T& e, bool* is_basic_type){
       *is_basic_type = false;
       Binder binder(boost::shared_ptr<BinderImpBase>(new JsonEncodeBinder()));

       typedef typename boost::mpl::if_c<has_member_function_setBind<void (T::*) (Binder*)>::value, 
           MemberFunctionCaller, RunTimeBinderCaller>::type CallerT;

       CallerT().call(e, &binder);

       //e.setBind(&binder);
       JsonEncodeBinder* json_encoder_binder = dynamic_cast<JsonEncodeBinder*>(binder.binder_imp.get());
       return json_encoder_binder->root;
    }

    template<typename T>
    Json::Value encodeWithForeginKey(T& e, bool* is_basic_type){
        *is_basic_type = false;
        bool dummy;
        std::string key = e->getKeyStr();
        Json::Value jv = encode(key, &dummy);
        return jv;
    }

    template<typename T>
    Json::Value encode(T* e, bool* is_basic_type){
        T& _e = *e;
        return this->encode(_e, is_basic_type);
    } 
private:
    Json::Value encode(const Json::Value e, bool* is_basic_type){
        return e;
    }
private:  //for basic type
    // bool 
    Json::Value encode(const bool e, bool* is_basic_type) {*is_basic_type = true; return Json::Value(e);}
    //int32_t
    Json::Value encode(const int32_t e, bool* is_basic_type) {*is_basic_type = true; Json::Int x = e; return Json::Value(x); }
    //int64_t 
    Json::Value encode(const int64_t e, bool* is_basic_type) {*is_basic_type = true;Json::Int64 x = e; return Json::Value(x); }
    //float 
    Json::Value encode(const float e, bool* is_basic_type){*is_basic_type = true;  return Json::Value(e); }
    //double 
    Json::Value encode(const double e, bool* is_basic_type){*is_basic_type = true;  return Json::Value(e); }
    //string
    Json::Value encode(const std::string& e, bool* is_basic_type) {*is_basic_type = true; return Json::Value(e);}
    Json::Value encode(      std::string& e, bool* is_basic_type) {*is_basic_type = true; return Json::Value(e);}
public:
    Json::Value root;
};

 

}

#endif
