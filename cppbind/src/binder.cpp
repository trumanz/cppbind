#include "../include/imp/binder.hpp"
using namespace cppbind;

 Json::Value Binder::getJson(){
        if(this->isEncode) {
            EncodeBinder* binder = (EncodeBinder*)this->binder_imp;
            return binder->getJson();
        } else {
            DecodeBinder* binder =  (DecodeBinder*)this->binder_imp;
             return binder->getJson();
        }
        
  }
void Binder::setJson(const Json::Value jv){
     if(this->isEncode) {
            EncodeBinder* binder = (EncodeBinder*)this->binder_imp;
             binder->setJson(jv);
        } else {
            DecodeBinder* binder =  (DecodeBinder*)this->binder_imp;
              binder->setJson(jv);
        }
}

 Binder::Binder(bool isEncode, Json::Value root){
        this->isEncode = isEncode;
        if(this->isEncode) {
           this->binder_imp =  new EncodeBinder();
        } else {
           this->binder_imp =  new DecodeBinder(root);
        }
}

Binder::~Binder(){
         if(this->isEncode) {
            EncodeBinder* binder = (EncodeBinder*)this->binder_imp;
            delete binder;
        } else {
            DecodeBinder* binder =  (DecodeBinder*)this->binder_imp;
            delete binder;
        }
}

