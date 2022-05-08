#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include "json/json.h"
#include "cppbind/JsonDecoder.hpp"
#include "cppbind/JsonEncoder.hpp"

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
       XObj* x = JsonDecoder().decode<XObj>(ss);
       delete x;
     }  catch ( exception::ParseErrorException& e) {
             message =  e.what();
     }
     ASSERT_EQ(message.find("json value not mapping") >=0, true);
};

