#include "../include/imp/binder.hpp"
using namespace cppbind;


Binder::Binder(){
    this->binder_imp =  boost::shared_ptr<BinderImpBase>(new JsonEncodeBinder());
}
Binder::Binder(const Json::Value& root){
    this->binder_imp =  boost::shared_ptr<BinderImpBase>(new JsonDecodeBinder(root));
}


Binder::Binder(const std::vector<std::string>& csv_row){
    this->binder_imp = boost::shared_ptr<BinderImpBase>(new CSVDecodeBinder(csv_row));
}

Json::Value Binder::getJson(){
     return binder_imp->getJson();
}

void Binder::setJson(const Json::Value jv){
     binder_imp->setJson(jv);
}

bool Binder::isEncode()
{
    if(NULL == dynamic_cast<JsonEncodeBinder*>(this->binder_imp.get())) {
         return false;
    }
    return true;
}

size_t Binder::getCSVUsed(){
    CSVDecodeBinder*  csv_decode_binder  = dynamic_cast<CSVDecodeBinder*>(this->binder_imp.get());
    return  csv_decode_binder->used;
}
