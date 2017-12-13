#ifndef _CPPBIND_Object_H
#define _CPPBIND_Object_H
#include <typeinfo>

namespace cppbind{


class Object{
public:
    virtual ~Object(){}
    virtual Json::Value getJsonValue4Bind() = 0; 
};


}

#endif

