#ifndef _CPPBIND_CLASS__REGISTER_H
#define _CPPBIND_CLASS__REGISTER_H
#include <typeinfo>
#include <boost/noncopyable.hpp>
#include "cppbind/object/ObjFactory.h"
#include "cppbind/object/ObjFactoryT.h"
#include "ClassRegisterBase.h"

namespace cppbind{ namespace dynamic {

class ClassRegister : public ClassRegisterBase {
public:
   template<typename ClassT>
   void regClass(const char *name, Json::Value property = Json::Value()){
       ObjFactory* of = new ObjFactoryT<ClassT>();
       of->obj_base_property = property;
       this->regClassWithFactory(name, of);
   }

};

}}

#endif

