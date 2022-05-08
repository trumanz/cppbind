inline void JsonDecoderImp::decode(const Json::Value& json, bool*e){
    if(json.isString()) {
        FundamentalType().decode_fundamental(json,e);
        return;
    }
    e[0] =  json.asBool();
}
inline void JsonDecoderImp::decode(const Json::Value& json, int32_t *e){
    if(json.isString()) {
        FundamentalType().decode_fundamental(json,e);
        return;
    }
    e[0] =  json.asInt();
}
inline void JsonDecoderImp::decode(const Json::Value& json, uint32_t *e){
    if(json.isString()) {
        FundamentalType().decode_fundamental(json,e);
        return;
    }
    e[0] =  json.asUInt();
}
inline void JsonDecoderImp::decode(const Json::Value& json, int64_t *e){
    if(json.isString()) {
        FundamentalType().decode_fundamental(json,e);
        return;
    }
    e[0] =  json.asInt64();
}
inline void JsonDecoderImp::decode(const Json::Value& json, uint64_t *e){
    if(json.isString()) {
        FundamentalType().decode_fundamental(json,e);
        return;
    }
    e[0] =  json.asUInt64();
}
inline void JsonDecoderImp::decode(const Json::Value& json, float *e){
    if(json.isString()) {
        FundamentalType().decode_fundamental(json,e);
        return;
    }
    e[0] =  json.asFloat();
}
inline void JsonDecoderImp::decode(const Json::Value& json, double *e){
    if(json.isString()) {
        FundamentalType().decode_fundamental(json,e);
        return;
    }
    e[0] =  json.asDouble();
}

inline void JsonDecoderImp::decode(const Json::Value& json, std::string *e){
    e[0] =  json.asString();
}