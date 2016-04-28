#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <jsoncpp/json/json.h>
#include "cppbind_json.hpp"

using namespace cppbind;

class Me2 {
public:
    int  not_existed;
    void setBind(Mapper &mapper){
          mapper.bind("not_existed", not_existed);
    }
   
};

TEST(JsonROM, child_not_exist_message){
     std::string message;
     try {
       std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
       boost::shared_ptr<Me2> me = JsonBind<Me2>().decode(ifs);
     }  catch ( CppBindException e) {
             message =  e.what();
     }
     ASSERT_EQ(".not_existed not found", message);
};


class Skill2 {
public:
    std::string language;
    boost::shared_ptr<int> grade;
    void setBind(Mapper &mapper) {
          mapper.bind("language_not_exist", language);
    }
};

class Me3 {
public:
    std::string name;
    std::map<std::string, Skill2>  skills;
    void setBind(Mapper &mapper){
          mapper.bind("name", name);
          mapper.bind("skills", skills);
    }
};


TEST(JsonROM, child_child_not_exist_message){
     std::string message;
     try {
       std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
       boost::shared_ptr<Me3> me = JsonBind<Me3>().decode(ifs);
     }  catch ( CppBindException e) {
             message = e.what();
     }
     ASSERT_EQ(".skills.R.language_not_exist not found", message);
};
