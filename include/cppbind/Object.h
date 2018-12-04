#ifndef _CPPBIND_Object_H
#define _CPPBIND_Object_H
#include <assert.h>
#include <typeinfo>
#include <jsoncpp/json/json.h>
namespace cppbind{

class Object{
public:
    virtual ~Object(){}
    //TODO delete getJsonValue4Bind
    virtual Json::Value getJsonValue4Bind() const { assert(false);return Json::Value();}
    virtual std::string getObjGenName() const { return typeid(*this).name(); }
    virtual Json::Value getObjProperty() const{ return Json::Value(); }
};

class ForeignObjFactory{
public:
    virtual Object* createObj(const std::string& key) const = 0;
};
template<typename T>
class ForeignObjFactoryT : public ForeignObjFactory {
public:
    virtual Object* createObj(const std::string& key) const 
    {
        T* obj = new T();
        obj->setKeyStr(key);
        return obj;
    }
};


}

#endif

