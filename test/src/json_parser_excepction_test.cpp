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

class Me2 {
public:
    int  not_existed;
    void setBind(Binder *binder){
          binder->bind("not_existed", not_existed);
    }
   
};

TEST(JsonROM, child_not_exist_message){
     std::string message;
     try {
       std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
       boost::shared_ptr<Me2> me = JsonBind().decode<Me2>(ifs);
     }  catch ( CppBindException e) {
             message =  e.what();
     }
     ASSERT_EQ("3Me2.not_existed not found", message);
};


class Skill2 {
public:
    std::string language;
    boost::shared_ptr<int> grade;
    void setBind(Binder *binder) {
          binder->bind("language_not_exist", language);
    }
};

class Me3 {
public:
    std::string name;
    std::map<std::string, Skill2>  skills;
    void setBind(Binder *binder){
          binder->bind("name", name);
          binder->bind("skills", skills);
    }
};


TEST(JsonROM, child_child_not_exist_message){
     std::string message;
     try {
       std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
       boost::shared_ptr<Me3> me = JsonBind().decode<Me3>(ifs);
     }  catch ( CppBindException e) {
             message = e.what();
     }
     ASSERT_EQ("3Me3.skills.R.language_not_exist not found", message);
};
