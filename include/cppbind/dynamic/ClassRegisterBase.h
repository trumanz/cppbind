﻿#ifndef _CPPBIND_CLASS__REGISTER_BASE_H
#define _CPPBIND_CLASS__REGISTER_BASE_H
#include <stdio.h>
#include <assert.h>
#include <typeinfo>
#include <boost/noncopyable.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include "cppbind/object/ObjFactory.h"
#include "cppbind/exception/cppbind_exception.h"

namespace cppbind{
using namespace object;
    namespace dynamic {
class ClassRegisterBase {
protected:
   std::map<std::string, boost::shared_ptr<ObjFactory> > obj_factories;
public:
   void regClassWithFactory(const std::string name, boost::shared_ptr<ObjFactory> obj_factory){
       std::string reg_name(name);
       boost::algorithm::to_lower(reg_name);
       if(this->obj_factories.count(reg_name)) {
           auto& r = *(this->obj_factories[reg_name].get());
           auto& r2 = *obj_factory;
           printf("Error %s alread registered %s when register %s", 
                  reg_name.c_str(),  typeid(r).name(),
                  typeid(r2).name());
           assert(false);
       } else {
           this->obj_factories[reg_name] = obj_factory;
       }
   }
   void regClassWithFactory(const std::string name, ObjFactory* obj_factory){
       this->regClassWithFactory(name,boost::shared_ptr<ObjFactory>(obj_factory));
   }

   template<typename ClassT>
   ClassT* createObj(const std::string& name, const Json::Value& json_parameter, JsonDecoderImp* bind = nullptr) const {
       Object* any_obj = this->createAnyObj(name,json_parameter, bind);
       ClassT* rc = dynamic_cast<ClassT*>(any_obj);
       assert(rc != nullptr);
       return rc; 
   }

   Object* createAnyObj(const std::string& name, const Json::Value& json_parameter, JsonDecoderImp* bind = nullptr) const {
       const ObjFactory* of = this->getObjFactory(name);
       Object* any_obj = of->createObj(name, json_parameter, bind);
       return any_obj;
   }
   void merge(const ClassRegisterBase& oth_reg) {
        for(std::map<std::string, boost::shared_ptr<ObjFactory> >::const_iterator it = oth_reg.obj_factories.begin();
              it != oth_reg.obj_factories.end(); it++){
            this->regClassWithFactory(it->first, it->second);
        }
   }
private:
   ObjFactory* getObjFactory(const std::string& name)const{
       std::string reg_name(name);
       boost::algorithm::to_lower(reg_name);
       std::map<std::string, boost::shared_ptr<ObjFactory> >::const_iterator it  = obj_factories.find(reg_name);
       if(it == obj_factories.end()) {
           printf("ERROR can not find class '%s'\n", name.c_str());
           throw exception::ClassMissRegException(name);
       }
       boost::shared_ptr<ObjFactory> obj_factory = it->second;
       return obj_factory.get();
   }
};


} }

#endif

