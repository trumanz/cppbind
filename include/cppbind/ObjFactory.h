#ifndef _CPPBIND_ObjFactory_H
#define _CPPBIND_ObjFactory_H
#include <typeinfo>
#include <boost/noncopyable.hpp>
#include <boost/any.hpp>
#include "Object.h"
namespace cppbind{

class JsonDecodeBinder;
class ObjFactory{
public:
    //any is a point
    virtual Object* createObj(const Json::Value& json_parameter, JsonDecodeBinder* bind = NULL) const = 0;
   // virtual boost::any createObj(const boost::any& any_parameter) const = 0;
    virtual std::string getTypeIdName() const = 0;
};


}

#endif

