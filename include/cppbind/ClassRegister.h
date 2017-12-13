#ifndef _CPPBIND_CLASS__REGISTER_H
#define _CPPBIND_CLASS__REGISTER_H
#include <typeinfo>
#include <boost/noncopyable.hpp>
#include "ObjFactory.h"
#include "ClassRegisterBase.h"

namespace cppbind{

class ClassRegister : public ClassRegisterBase {
public:
   template<typename ClassT>
   void regClass(const char *name){
       ObjFactory* of = new ObjFactoryT<ClassT>();
       this->regClassWithFactory(name, of);
   }

};


}

#endif

