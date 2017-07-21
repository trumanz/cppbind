#ifndef __CPPBIND__JSONBIND_IMP__
#define __CPPBIND__JSONBIND_IMP__
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <jsoncpp/json/json.h>
#include <assert.h>
#include <list>
#include <boost/shared_ptr.hpp>
#include <stdint.h>
#include "../cppbind_exception.h"


namespace  cppbind {

class BinderImpBase {
public:
     virtual ~BinderImpBase() {}
};

class Binder{
public:
    //default is encoder
    Binder();
    //decoder
    Binder(const Json::Value& root);
    Binder(const std::vector<std::string>& csv_row);

    /*
    *User code will call bind to build the attibute and name relation. 
    *Because the user code is not template functon, so we must tranfer a fixed type, This Binder type!!
    *and this will use EncodeBinder or DecodeBinder  to do the real work.
    */
    template<typename T>
    void bind(const std::string& name, T& v);
 public:
    boost::shared_ptr<BinderImpBase> binder_imp;
};

}

#include "imp/json_decode_binder.hpp"
#include "imp/json_encode_binder.hpp"
#include "imp/csv_decode_binder.hpp"


namespace cppbind {

template<typename T>
void Binder::bind(const std::string& name, T& v){
        //CALL the real bind fucntion, must change to the real type of binder, then comile could instantiate the tempate funciton
        JsonEncodeBinder* json_encode_binder = dynamic_cast<JsonEncodeBinder*>(this->binder_imp.get());
        JsonDecodeBinder* json_decode_binder = dynamic_cast<JsonDecodeBinder*>(this->binder_imp.get());
        CSVDecodeBinder*  csv_decode_binder  = dynamic_cast<CSVDecodeBinder*>(this->binder_imp.get());
        if( json_encode_binder ) {
            json_encode_binder->bind(name,v);
        } else if(json_decode_binder ) {
            json_decode_binder->bind(name,v);
        } else if(csv_decode_binder) {
            csv_decode_binder->bind(name,v);
        } else {
            assert("bug" == NULL);
        }
}


}

#endif