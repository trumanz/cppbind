#ifndef _CPPBIND_ObjFactory_H
#define _CPPBIND_ObjFactory_H
#include <typeinfo>
#include <boost/noncopyable.hpp>
#include <boost/any.hpp>
#include "Object.h"
namespace cppbind{

class JsonDecoderImp;
class ObjFactory{
public:
    virtual ~ObjFactory(){}
    //any is a point
    virtual Object* createObj(const std::string& obj_name, const Json::Value& json_parameter, JsonDecoderImp* bind = NULL) const = 0;
   // virtual boost::any createObj(const boost::any& any_parameter) const = 0;
    virtual std::string getTypeIdName() const = 0;
    Json::Value obj_base_property;
};


}

#endif

