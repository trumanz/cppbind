template<typename T>
void JsonDecoderImp::decodeDynamicType(const Json::Value& _jv, const std::string& name, T*& v, Json::Value* default_value ){
    Json::Value jv = _jv[name];
    if(jv.isNull() && default_value != nullptr) {
        jv = *default_value;
    }
    decodeDynamicType(jv, v);
}

template<typename T>
void JsonDecoderImp::decodeDynamicType(const Json::Value& _jv, const std::string& name, boost::shared_ptr<T> &v, boost::shared_ptr<T> default_value){
    Json::Value jv = _jv[name];
    if(jv.isNull()) {
        v = default_value;
    } else {
        T* tmp_v = nullptr;
        decodeDynamicType(jv, tmp_v);
        v = boost::shared_ptr<T>(tmp_v);
    }
}

template<typename T>
void JsonDecoderImp::decodeDynamicType(const Json::Value& jv, T*& v){
    Json::Value::Members  members = jv.getMemberNames();
    if(members.size() != 1) {
        throw exception::ParseErrorException("DynamicType object must have a unique key as it's type");
    }
    std::string class_name = members[0];
    Json::Value class_data = jv[class_name];

    if(this->class_reg == nullptr) {
        throw exception::ParseErrorException("no class register");
    }
    v = this->class_reg->createObj<T>(class_name.c_str(), class_data, this);
}
template<typename T>
void JsonDecoderImp::decodeDynamicType(const Json::Value &_jv, const std::string& name, std::vector<T*>& v, Json::Value* default_value){
    Json::Value jv = _jv[name];
    if(jv.isNull() && default_value != nullptr) {
        jv = *default_value;
    }
    if(!jv.isArray()) {
        throw exception::ParseErrorException(name, "should be a object array");
    }
    for(Json::Value::ArrayIndex i = 0; i  < jv.size(); i++) {
        T* e;
        if(!jv[i].isObject()) {
            throw exception::ParseErrorException(name, "should be a object");
        }
        try {
            decodeDynamicType(jv[i], e);
            v.push_back(e);
        } catch (exception::ClassMissRegException& e) {
            if (!this->ignore_unknown_key) {
                throw e;
            }
        } catch(exception::ParseErrorException& e) {
            std::stringstream ss; ss << i;
            e.addParentNodeName(ss.str());
            throw e;
        }
    }
}
