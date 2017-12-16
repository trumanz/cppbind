#ifndef __JSON_ENCODE_BINDER_H__
#define __JSON_ENCODE_BINDER_H__

namespace  cppbind {

class JsonEncodeBinder : public BinderImpBase {
private:
    ClassRegisterBase* class_reg;
public:
    JsonEncodeBinder(ClassRegisterBase* _class_reg){
        this->class_reg = _class_reg;
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
    template<typename T>
    void bindWithDynamicType(const std::string& name, T*& v){
        //printf("name=%s\n", name.c_str());
        std::string typid_name = typeid(*v).name();
        std::string reg_name = this->class_reg->getRegName(typid_name.c_str());
        Json::Value jv;
        jv[reg_name] = v->getJsonValue4Bind();
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
            KeyT k = it->first;
            Json::Value k_jv = encode(k, &is_basic_type);
            Json::Value v_jv = encode(it->second, &dummy);
            
            std::string str_key = k_jv.toStyledString();
            std::size_t x = str_key.find_last_not_of("\r\n");
            if(x!= std::string::npos) {
                str_key = str_key.substr(0, x+1);
            }
            if(k_jv.isString()) {
                assert(str_key.length() >= 2);
                assert(str_key[0] == '\"');
                assert(str_key[str_key.length()-1] == '\"');
                str_key = str_key.substr(1, str_key.length()-2);
            }
            jv[str_key] = v_jv;
            
        }
        return jv;
    }
//CLASS&STRUCT type
public:
    
    class SetBindMemberFunctionCaller{
    public:
        template<typename T>
        void call(T&e, Binder* binder){
            e.setBind(binder);
        }
    };

    class ToJsonValue4BindMemberFunctionCaller{
    public:
        template<typename T>
        void call(T&e, Binder* binder){
            Json::Value jv = e.toJsonValue4Bind();
            JsonEncodeBinder* json_encoder_binder = dynamic_cast<JsonEncodeBinder*>(binder->binder_imp.get());
            json_encoder_binder->setJson(jv);
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
    class Option2MemberFunctionCaller{
    public:
        template<typename T>
        void call(T&e, Binder* binder){
          //if have "Json::Value toJsonValue4Bind()" then call setbind; else call otheres
          typedef typename boost::mpl::if_c<has_member_function_toJsonValue4Bind<Json::Value (T::*) ()>::value, 
              ToJsonValue4BindMemberFunctionCaller, RunTimeBinderCaller>::type CallerT;

          CallerT().call(e, binder); 
        }
    };
     
    template<typename T>
    Json::Value encode(T& e, bool* is_basic_type){
       *is_basic_type = false;
       Binder binder(boost::shared_ptr<BinderImpBase>(new JsonEncodeBinder(this->class_reg)));

       typedef typename boost::mpl::if_c<has_member_function_setBind<void (T::*) (Binder*)>::value, 
           SetBindMemberFunctionCaller, Option2MemberFunctionCaller>::type CallerT;

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
