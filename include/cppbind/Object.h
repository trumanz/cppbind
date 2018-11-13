#ifndef _CPPBIND_Object_H
#define _CPPBIND_Object_H
#include <typeinfo>
#include <jsoncpp/json/json.h>
namespace cppbind{

class Object{
public:
    virtual ~Object(){}
    virtual Json::Value getJsonValue4Bind() const = 0; 
    virtual std::string getObjGenName() const { return typeid(*this).name(); }
    virtual Json::Value getObjProperty() const{
        return Json::Value();
    }
};


}

#endif

