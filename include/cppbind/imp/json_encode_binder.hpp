#ifndef __JSON_ENCODE_BINDER_H__
#define __JSON_ENCODE_BINDER_H__

namespace  cppbind {

class JsonEncodeBinder : public BinderImpBase {
public:
    Binder binder;
public:
    JsonEncodeBinder(){
        binder.init(this, NULL);
    }
   // void setJson(const Json::Value& jv){
   //     this->root_jv[0] = jv;
   // }
    template<typename T>
    void bind(Json::Value *_jv, const std::string& name, T& v){
          Json::Value jv;
          encode(v, &jv);
          _jv[0][name] = jv;
    }

    template<typename T>
    void bind(Json::Value *_jv, const std::string& name, T& v, const T& default_value){
          this->bind(_jv, name,v);
    }

    template<typename T>
    void bind(Json::Value *_jv, const std::string& name, boost::shared_ptr<T>& v){
          if(v.get() != NULL) {
              this->bind(_jv, name,*(v.get()));
          }
    }

    template<typename T>
    void bindWithForeginKey(Json::Value *_jv, const std::string& name, T& v){
          Json::Value jv;
          encodeWithForeginKey(v, &jv);
          _jv[0][name] = jv;
    }
    template<typename T>
    void bindWithDynamicType(Json::Value *_jv, const std::string& name, T*& v){
        //printf("name=%s\n", name.c_str());
        std::string typid_name = typeid(*v).name();
        std::string reg_name = binder_data.class_reg->getRegName(typid_name.c_str());
        Json::Value jv;
        jv[reg_name] = v->getJsonValue4Bind();
        _jv[0][name] = jv;
    }
    
public: //for std container type
    
    template<typename T>
    void encode(std::vector<T>& e, Json::Value* jv){
        if(e.size() == 0) {
            jv[0] =  Json::Value(Json::arrayValue);
        }
        for(typename std::vector<T>::iterator  it  = e.begin(); it != e.end(); it++) {
             Json::Value je;
             encode(*it, &je);
             jv->append(je);
        }
    }
    template<typename T>
    void encode(std::vector<T*>& e_list, Json::Value* jv){
        if(e_list.size() == 0) {
            jv[0] =  Json::Value(Json::arrayValue);
        }
        for(typename std::vector<T*>::iterator  it  = e_list.begin(); it != e_list.end(); it++) {
             Json::Value je;
             T* e = *it;
             encode(*(e), &je);
             jv->append(je);
        }
    }
    template<typename T>
    void encodeWithForeginKey(std::vector<T*>& e, Json::Value* jv){
        if(e.size() == 0) {
            jv[0] =  Json::Value(Json::arrayValue);
        }
        for(typename std::vector<T*>::iterator  it  = e.begin(); it != e.end(); it++) {
             T* x = *it;
             Json::Value je;
             encode(x->getKeyStr(), &je);
             jv->append(je);
        }
    }
    template<typename T>
    void encode(std::list<T*>& e, Json::Value* jv){
        for(typename std::list<T*>::iterator  it  = e.begin(); it != e.end(); it++) {
             T& e_x = *(*it);
             Json::Value je = encode(e_x);
             jv->append(je);
        }
    }
    template<typename T>
    void encode(std::list<T>& e, Json::Value* jv){
        for(typename std::list<T>::iterator  it  = e.begin(); it != e.end(); it++) {
             Json::Value je;
             encode(*it, &je);
             jv->append(je);
        }
    }
    template<typename T>
    void encode(std::set<T>& e_set, Json::Value* jv){
        if(e_set.empty()) jv[0] = Json::arrayValue;
        for(typename std::set<T>::iterator  it  = e_set.begin(); it != e_set.end(); it++) {
             Json::Value je;
             encode(*(T*)(&(*it)), &je);
             //encode(e, &je);
             jv->append(je);
        }
    }

   
    template<typename KeyT, typename ValueT>
    void encode(std::map<KeyT, ValueT>& e, Json::Value* _jv){
        Json::Value jv;
        for(typename std::map<KeyT, ValueT>::iterator it =  e.begin(); it != e.end(); it++) {
            KeyT k = it->first;
            Json::Value k_jv;
            encode(k, &k_jv);
            Json::Value v_jv;
            encode(it->second, &v_jv);
            
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
        _jv[0] = jv;
    }
//CLASS&STRUCT type
public:
    //call setBind
    class SetBindMemberFunctionCaller{
    public:
        template<typename T>
        void call(T&e, JsonEncodeBinder* jbinder, Json::Value* jv){
            Binder binder;
            binder.init(jbinder, jv);
            e.setBind(&binder, false);
        }
    };

    //call toJsonValue4Bind
    class ToJsonValue4BindMemberFunctionCaller{
    public:
        template<typename T>
        void call(T&e, JsonEncodeBinder* binder, Json::Value* jv){
            jv[0] = e.toJsonValue4Bind();
        }
    };

    //call registered functon
    class RunTimeBinderCaller{
    public:
        template<typename T>
        void call(T&e, JsonEncodeBinder* binder, Json::Value* jv){
            std::string x = binder->binder_data.str_convert_mgmt.toString(e);
            jv[0] = Json::Value(x);
        }
    };

    //call toJsonValue4Bind or RunTime Registered
    class Option2MemberFunctionCaller{
    public:
        template<typename T>
        void call(T&e, JsonEncodeBinder* binder, Json::Value* jv){
          //if have "Json::Value toJsonValue4Bind()" then call setbind; else call otheres
          typedef typename boost::mpl::if_c<has_member_function_toJsonValue4Bind<Json::Value (T::*) ()>::value, 
              ToJsonValue4BindMemberFunctionCaller, RunTimeBinderCaller>::type CallerT;

          CallerT().call(e, binder, jv); 
        }
    };
    //call setbind or  Option2MemberFunctionCaller
    template<typename T>
    void encode(T& e, Json::Value* jv){

       typedef typename boost::mpl::if_c<has_member_function_setBind<void (T::*) (Binder*, bool)>::value, 
           SetBindMemberFunctionCaller, Option2MemberFunctionCaller>::type CallerT;

       CallerT().call(e, this, jv);
    }

    template<typename T>
    void encodeWithForeginKey(T& e, Json::Value* jv){
         encode(e->getKeyStr(), jv);
    }

    template<typename T>
    void encode(T* e, Json::Value* jv){
        T& _e = *e;
        this->encode(_e, jv);
    } 

    template<typename T>
    void encodeToJson(T* e, Json::Value* jv){
        this->binder.init(this, jv);
        T& _e = *e;
        this->encode(_e, jv);
    } 

private:
    void encode(const Json::Value& e, Json::Value* jv){
        jv[0] = e;
    }
    void encode(Json::Value& e, Json::Value* jv){
        jv[0] = e;
    }
//for basic type
private:  
    // bool 
    void encode(const bool e, Json::Value* jv) { jv[0] = Json::Value(e);}
    //int32_t
    void encode(const int32_t e, Json::Value* jv) { Json::Int x = e; jv[0] =  Json::Value(x); }
    //int64_t 
    void encode(const int64_t e, Json::Value* jv) { Json::Int64 x = e; jv[0] =  Json::Value(x); }
    //float 
    void encode(const float e, Json::Value* jv){ jv[0] =  Json::Value(e); }
    //double 
    void encode(const double e, Json::Value* jv){ jv[0] =  Json::Value(e); }
    //string
    void encode(const std::string& e, Json::Value* jv) { jv[0] =  Json::Value(e); }
    void encode(      std::string& e, Json::Value* jv) { jv[0] =  Json::Value(e); }
};

 

}

#endif
