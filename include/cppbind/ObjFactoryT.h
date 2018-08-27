#ifndef _CPPBIND_ObjFactoryT_H
#define _CPPBIND_ObjFactoryT_H


#include "ObjFactory.h"
#include <cppbind/cppbind_json.hpp>

namespace cppbind{


template<typename ObjT>
class ObjWrapperT : public ObjT
{
private:
    std::string reg_class_name;
public:
    ObjWrapperT(const std::string& class_name, const typename ObjT::Data4Bind& data) : ObjT(data){
        this->reg_class_name = class_name;
    }
    virtual Json::Value getJsonValue4Bind() const {
        typename ObjT::Data4Bind data = this->getData4Bind();
        return cppbind::JsonBind().encodeToJsonValue(data);
    }
    std::string getRegClassName() const {
        return reg_class_name;
    }

};

template<typename ObjT>
class ObjFactoryT : public ObjFactory
{
private:
    std::string reg_class_name;
public:
    ObjFactoryT(const std::string& class_name) {
        this->reg_class_name = class_name;
    }
    virtual Object* createObj(const Json::Value& json_parameter) const {
         boost::shared_ptr<typename ObjT::Data4Bind> parameter = cppbind::JsonBind().decode<typename ObjT::Data4Bind>(json_parameter);
         //ObjT* p = new ObjT(*(parameter.get()));
         Object* p = new ObjWrapperT<ObjT>(reg_class_name, *(parameter.get()));
         return p;
    }
    virtual std::string getTypeIdName() const{
        //return typeid(ObjT).name();
        return typeid(ObjWrapperT<ObjT>).name();
    }
};


}

#endif

