#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <jsoncpp/json/json.h>
#include <assert.h>
#include <list>
#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include "json_imp.hpp"
namespace  cppbind {

template<typename T>
class JsonBind{
public:
    boost::shared_ptr<T> get(std::istream &is){
          T* e = NULL;
         Json::Value root;
         Json::Reader reader;
         bool parsingSuccessful = reader.parse(is, root);
   
         if(!parsingSuccessful) {
           printf("Failed to parse, %s\n", reader.getFormatedErrorMessages().c_str());
           throw  CppOrmException(reader.getFormatedErrorMessages());
         } else {
             e = new T;
             Mapper mapper(root, false);
             e->setORM(mapper);
         }
         return boost::shared_ptr<T>(e);
    }
};


}
