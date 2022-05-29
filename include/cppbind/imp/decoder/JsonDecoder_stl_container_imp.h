//vector

template<typename T>
void JsonDecoderImp::decode(const Json::Value& json, std::vector<T>* e){
    std::list<T> v;
    decode(json, &v);
    e->clear();
    e->insert(e->begin(), v.begin(), v.end());
}

template<typename T>
void JsonDecoderImp::decode(const Json::Value& json, std::vector<T*>* e){
    std::list<T*> v;
    decode(json, &v);
    e->clear();
    e->insert(e->begin(), v.begin(), v.end());
}
template<typename T>
void JsonDecoderImp::decodeWithForeginKey(const Json::Value& json, std::vector<T*>* e){
    std::vector<std::string> keys;
    if(!json.isArray()) {
        throw exception::ParseErrorException("should be a list");
    }
    for(Json::Value::ArrayIndex i = 0; i  < json.size(); i++) {
        T* x = nullptr;
        decodeWithForeginKey(json[i], &x);
        e->push_back(x);
    }
}
//list
template<typename T>
void JsonDecoderImp::decode(const Json::Value& json, std::list<T>* e){
    if(!json.isArray()) {
        throw exception::ParseErrorException("should be a list");
    }
    e->clear();
    for(Json::Value::ArrayIndex i = 0; i  < json.size(); i++) {
        try {
            T tmp;
            decode(json[i], &tmp);
            e->push_back(tmp);
        } catch  (exception::ParseErrorException& e) {
            std::stringstream ss;
            ss << "[" << i << "]";
            e.addParentNodeName(ss.str());
            throw e;
        }
    }
}
//list
template<typename T>
void JsonDecoderImp::decode(const Json::Value& json, std::list<T*>* e){
    if(!json.isArray()) {
        throw exception::ParseErrorException("should be a list");
    }
    e->clear();
    for(Json::Value::ArrayIndex i = 0; i  < json.size(); i++) {
        try {
            T*  tmp = new T();
            decode(json[i], tmp);
            e->push_back(tmp);
        } catch  (exception::ParseErrorException& e) {
            std::stringstream ss;
            ss << "[" <<  i << "]";
            e.addParentNodeName(ss.str());
            throw e;
        }
    }
}
//set
template<typename T>
void JsonDecoderImp::decode(const Json::Value& json, std::set<T>* e){
    std::list<T> v;
    decode(json, &v);
    for(typename std::list<T>::iterator it = v.begin(); it != v.end(); it++) {
        e->insert(*it);
    }
}

//map
template<typename KeyT, typename ValueT>
void JsonDecoderImp::decode(const Json::Value& json, std::map<KeyT, ValueT>* e){

    if(!json.isObject()) {
        throw exception::ParseErrorException("shoulbe be a object");
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
        } catch  (exception::ParseErrorException& e) {
            e.addParentNodeName(*it);
            throw e;
        }
    }
}
