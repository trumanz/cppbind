#include "../include/imp/binder.hpp"
using namespace cppbind;


Binder::Binder(){
    this->binder_imp =  boost::shared_ptr<JsonBinderBase>(new EncodeBinder());
}
Binder::Binder(const Json::Value& root){
    this->binder_imp =  boost::shared_ptr<JsonBinderBase>(new DecodeBinder(root));
}

Json::Value Binder::getJson(){
     return binder_imp->getJson();
}

void Binder::setJson(const Json::Value jv){
     binder_imp->setJson(jv);
}

bool Binder::isEncode()
{
   return NULL != dynamic_cast<EncodeBinder*>(this->binder_imp.get());
}
