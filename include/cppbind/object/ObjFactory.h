#ifndef _CPPBIND_ObjFactory_H
#define _CPPBIND_ObjFactory_H
#include <typeinfo>
#include <boost/noncopyable.hpp>
#include <boost/any.hpp>
#include "cppbind/object/Object.h"
namespace cppbind{
    namespace imp{
      class JsonDecoderImp;
    }
    using namespace imp;
    namespace object {

class ObjFactory{
public:
    virtual ~ObjFactory() = default;
    //any is a point
    virtual Object* createObj(const std::string& obj_name, const Json::Value& json_parameter, JsonDecoderImp* bind = nullptr) const = 0;
   // virtual boost::any createObj(const boost::any& any_parameter) const = 0;
    virtual std::string getTypeIdName() const = 0;
    Json::Value obj_base_property;
};


}}

#endif

