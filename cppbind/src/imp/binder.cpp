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
 
