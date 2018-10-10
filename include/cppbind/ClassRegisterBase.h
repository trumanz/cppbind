#ifndef _CPPBIND_CLASS__REGISTER_BASE_H
#define _CPPBIND_CLASS__REGISTER_BASE_H
#include <stdio.h>
#include <assert.h>
#include <typeinfo>
#include <boost/noncopyable.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include "ObjFactory.h"
#include "cppbind_exception.h"

namespace cppbind{

class ClassRegisterBase {
private:
   std::map<std::string, ObjFactory*> obj_factories;
public:

   void regClassWithFactory(const char *name, ObjFactory* obj_factory){
       std::string reg_name(name);
       boost::algorithm::to_upper(reg_name);
       if(this->obj_factories.find(reg_name) != this->obj_factories.end()) {
           printf("Error %s alread registered", reg_name.c_str());
           assert(false);
       } else {
           this->obj_factories[reg_name] = obj_factory;
       }
   }

   template<typename ClassT>
   ClassT* createObj(const char* name, const Json::Value& json_parameter, JsonDecodeBinder* bind = NULL)  {
       Object* any_obj = this->createAnyObj(name,json_parameter, bind);
       ClassT* rc = dynamic_cast<ClassT*>(any_obj);
       assert(rc != NULL);
       return rc; 
   }

   Object* createAnyObj(const char* name, const Json::Value& json_parameter, JsonDecodeBinder* bind = NULL)  {
       ObjFactory* of = this->getObjFactory(name);
       Object* any_obj = of->createObj(name, json_parameter, bind);
       return any_obj;
   }
private:
   ObjFactory* getObjFactory(const char* name){
       std::string reg_name(name);
       boost::algorithm::to_upper(reg_name);
       std::map<std::string, ObjFactory*>::iterator it  = obj_factories.find(reg_name);
       if(it == obj_factories.end()) {
           printf("ERROR can not find class %s\n", name);
           throw ClassMissRegException(name);
       }
       return it->second;
   }
};


}

#endif

