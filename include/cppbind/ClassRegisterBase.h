#ifndef _CPPBIND_CLASS__REGISTER_BASE_H
#define _CPPBIND_CLASS__REGISTER_BASE_H
#include <stdio.h>
#include <assert.h>
#include <typeinfo>
#include <boost/noncopyable.hpp>
#include "ObjFactory.h"
#include "cppbind_exception.h"

namespace cppbind{

class ClassRegisterBase {
private:
   std::map<std::string, ObjFactory*> obj_factories;
   std::map<std::string, std::string> typeid_name_str_name_map;
public:

   void regClassWithFactory(const char *name, ObjFactory* obj_factory){
       this->obj_factories[name] = obj_factory;
       this->typeid_name_str_name_map[obj_factory->getTypeIdName()] = name;
   }

   std::string getRegName(const char* typeid_name){
       const std::map<std::string, std::string>::const_iterator it = typeid_name_str_name_map.find(typeid_name);
       if(it == typeid_name_str_name_map.end()) {
           printf("ERROR can not found class typeid name: %s\n", typeid_name);
           for(std::map<std::string, std::string>::const_iterator it2 = typeid_name_str_name_map.begin();
                it2 !=  typeid_name_str_name_map.end(); it2++){
               printf("registered class typeid name: %s\n", it2->first.c_str());
           }
           assert(false);
       }
       return it->second;
   }


   template<typename ClassT>
   ClassT* createObj(const char* name, const Json::Value& json_parameter)  {
       Object* any_obj = this->createAnyObj(name,json_parameter);
       ClassT* rc = dynamic_cast<ClassT*>(any_obj);
       assert(rc != NULL);
       return rc; 
   }

   Object* createAnyObj(const char* name, const Json::Value& json_parameter)  {
       ObjFactory* of = this->getObjFactory(name);
       Object* any_obj = of->createObj(json_parameter);
       return any_obj;
   }
private:
   ObjFactory* getObjFactory(const char* name){
       std::map<std::string, ObjFactory*>::iterator it  = obj_factories.find(name);
       if(it == obj_factories.end()) {
           printf("ERROR can not find class %s\n", name);
           throw ClassMissRegException(name);
       }
       return it->second;
   }
};


}

#endif

