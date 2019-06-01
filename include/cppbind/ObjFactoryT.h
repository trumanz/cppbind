#ifndef _CPPBIND_ObjFactoryT_H
#define _CPPBIND_ObjFactoryT_H


#include "ObjFactory.h"
#include <cppbind/jsonbind.hpp>

namespace cppbind{


template<typename ObjT>
class ObjWrapperT : public ObjT
{
private:
    std::string obj_gen_name;
    Json::Value obj_property;
public:
    ObjWrapperT(std::string obj_name,const Json::Value& _property,
                const typename ObjT::Data4Bind& data) : ObjT(data){
        this->obj_gen_name = obj_name;
        this->obj_property = _property;
    }
    virtual Json::Value getObjProperty()const{
        return obj_property;
    }
    virtual Json::Value getJsonValue4Bind() const {
        typename ObjT::Data4Bind data = this->getData4Bind();
        Json::Value jv;
        jv[obj_gen_name] = cppbind::JsonBind().encodeToJsonValue(data);
        return jv;
    }
    std::string getObjGenName() const {
        return this->obj_gen_name;
    }

};

template<typename ObjT>
class ObjFactoryT : public ObjFactory
{
public:
    ObjFactoryT() {
    }
    virtual Object* createObj(const std::string& obj_name, const Json::Value& json_parameter, JsonDecodeBinder* bind = NULL) const {
         boost::shared_ptr<typename ObjT::Data4Bind> parameter;
         if(bind) {
              typename ObjT::Data4Bind* e = NULL;
              e = new typename ObjT::Data4Bind();
              bind->DecodeJson((Json::Value*)&json_parameter, e);
              parameter =  boost::shared_ptr<typename ObjT::Data4Bind>(e);
         } else {
             parameter = boost::shared_ptr<typename ObjT::Data4Bind>(cppbind::JsonBind().decode<typename ObjT::Data4Bind>(json_parameter));
         }
         //ObjT* p = new ObjT(*(parameter.get()));
         Json::Value  _property;
         _property["baseProperty"] = this->obj_base_property;
         _property["originParameter"] = json_parameter;
         Object* p = new ObjWrapperT<ObjT>(obj_name,_property, *(parameter.get()));
         return p;
    }
    virtual std::string getTypeIdName() const{
        //return typeid(ObjT).name();
        return typeid(ObjWrapperT<ObjT>).name();
    }
};


}

#endif

