#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <jsoncpp/json/json.h>
#include "cppbind/jsonbind.hpp"

using namespace cppbind;

class XObj {
public:
    int  x;
    void setBind(Binder *binder, bool load){
          binder->bind("x",x);
    }
   
};

TEST(JsonROM, unknown_member){
     std::string message;
     try {
       std::stringstream ss("{\"x\": 1, \"y\":2}");
       XObj* x = JsonBind().decode<XObj>(ss);
       delete x;
     }  catch ( ParseErrorException e) {
             message =  e.what();
     }
     ASSERT_EQ(" unknown json key:[y]", message);
};

