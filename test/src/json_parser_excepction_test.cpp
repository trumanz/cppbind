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
#include <boost/filesystem.hpp>

using namespace cppbind;

class Me2 {
public:
    int  not_existed;
    void setBind(Binder *binder, bool load){
          binder->bind("not_existed", not_existed);
    }
   
};

TEST(JsonROM, child_not_exist_message){
     std::string message;
     try {
         boost::filesystem::path path = __FILE__;
         path = path.parent_path().parent_path()/"sample_data/me.json";

       //std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
         std::ifstream ifs(path.c_str(),  std::ifstream::in);
       Me2* me = JsonDecoder().decode<Me2>(ifs);
       delete me;
     }  catch ( exception::ParseErrorException& e) {
             message =  e.what();
     }
     ASSERT_EQ("not_existed not found", message);
};


class Skill2 {
public:
    std::string language;
    boost::shared_ptr<int> grade;
    void setBind(Binder *binder,  bool load) {
          binder->bind("language_not_exist", language);
    }
};

class Me3 {
public:
    std::string name;
    std::map<std::string, Skill2>  skills;
    void setBind(Binder *binder,  bool load){
          binder->bind("name", name);
          binder->bind("skills", skills);
    }
};


TEST(JsonROM, child_child_not_exist_message){
     std::string message;
     try {
         boost::filesystem::path path = __FILE__;
         path = path.parent_path().parent_path()/"sample_data/me.json";
         std::ifstream ifs(path.c_str(),  std::ifstream::in);
      // ASSERT_EQ(ifs.is_open(), true) << "test";
       ASSERT_TRUE(ifs.is_open()) <<  "Path:" <<  path ;
       Me3* me = JsonDecoder().decode<Me3>(ifs);
       delete me;
     }  catch ( exception::ParseErrorException& e) {
             message = e.what();
     }
     ASSERT_EQ("skills.R.language_not_exist not found", message);
};
