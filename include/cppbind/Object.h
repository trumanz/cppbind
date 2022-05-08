#ifndef _CPPBIND_Object_H
#define _CPPBIND_Object_H
#include <assert.h>
#include <typeinfo>
#include <json/json.h>
namespace cppbind{

class Object{
public:
    virtual ~Object(){}
    //TODO delete getJsonValue4Bind
    virtual Json::Value getJsonValue4Bind() const { assert(false);return Json::Value();}
    virtual std::string getObjGenName() const { return typeid(*this).name(); }
    /**
		Make this cat look super cute
	*/
    virtual Json::Value getObjProperty() const{ return Json::Value(); }
};


}


#endif

