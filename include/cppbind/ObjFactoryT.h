#ifndef _CPPBIND_ObjFactoryT_H
#define _CPPBIND_ObjFactoryT_H


#include "ObjFactory.h"
#include <cppbind/cppbind_json.hpp>

namespace cppbind{


template<typename ObjT>
class ObjFactoryT : public ObjFactory
{
public:
    virtual Object* createObj(const Json::Value& json_parameter) const {
         boost::shared_ptr<typename ObjT::Data4Bind> parameter = cppbind::JsonBind().decode<typename ObjT::Data4Bind>(json_parameter);
         ObjT* p = new ObjT(*(parameter.get()));
         return p;
    }
    virtual std::string getTypeIdName() const{
        return typeid(ObjT).name();
    }
};


}

#endif

