#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <jsoncpp/json/json.h>
#include "cppbind/cppbind_json.hpp"

using namespace cppbind;

class XObj {
public:
    int  x;
    void setBind(Binder *binder){
          binder->bind("x",x);
    }
   
};

TEST(JsonROM, unknown_member){
     std::string message;
     try {
       std::stringstream ss("{\"x\": 1, \"y\":2}");
       XObj* x = JsonBind().decodeIStream2Point<XObj>(ss);
       delete x;
     }  catch ( CppBindException e) {
             message =  e.what();
     }
     ASSERT_EQ(" unknown json key:[y]", message);
};

