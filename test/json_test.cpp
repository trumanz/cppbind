#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <jsoncpp/json/json.h>
#include "json.hpp"

using namespace cppbind;

class Contact {
public:
    Contact(){}
    std::string email;
    std::string phone;
    void setORM(Mapper &mapper){
          mapper.orm("email", email);
          mapper.orm("phone", phone);
    }

};

class Skill {
public:
    int grade;
    void setORM(Mapper &mapper) {
          mapper.orm("grade", grade);
    }
};

class Me {
public:
    std::string name; //basic type
    int age;  //
    Contact contact; // class object
    std::list<std::string>  likes;  // std list
    std::map<std::string, Skill>  skills; // class list
    boost::shared_ptr<int>  value_not_exist; //optional not exist
    void setORM(Mapper &mapper){
          mapper.orm("name", name);
          mapper.orm("age", age);
          mapper.orm("contact", contact);
          mapper.orm("likes", likes);
          mapper.orm("skills", skills);
          mapper.orm("value_not_exist",  value_not_exist);
    }
};

TEST(JsonROM, baisc){

     std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
     boost::shared_ptr<Me> me = JsonBind<Me>().get(ifs);
     //basic type
     ASSERT_EQ(me->name ,"truman");
     ASSERT_EQ(me->age ,30);

     //optional 
     ASSERT_EQ(me->value_not_exist.get() , (int*)NULL);

     //std string list
     std::list<std::string>  likes = me->likes;
     ASSERT_EQ(likes.size(), 2);
     ASSERT_EQ(likes.front(), "Batman");
     ASSERT_EQ(likes.back(), "Superman");

      //  class  object
     Contact contact = me->contact;
     ASSERT_EQ(contact.email, "@com");
     ASSERT_EQ(contact.phone, "123456");

     // class list
     std::map<std::string,  Skill>  skills = me->skills;
     ASSERT_EQ(skills.size(), 2);
     ASSERT_EQ(skills["c++"].grade, 7);
     ASSERT_EQ(skills["R"].grade, 0);
};

class Me2 {
public:
    int  not_existed;
    void setORM(Mapper &mapper){
          mapper.orm("not_existed", not_existed);
    }
   
};

TEST(JsonROM, child_not_exist_message){
     std::string message;
     try {
       std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
       boost::shared_ptr<Me2> me = JsonBind<Me2>().get(ifs);
     }  catch ( CppOrmException e) {
             message =  e.what();
     }
     ASSERT_EQ(".not_existed not found", message);
};


class Skill2 {
public:
    std::string language;
    boost::shared_ptr<int> grade;
    void setORM(Mapper &mapper) {
          mapper.orm("language_not_exist", language);
    }
};

class Me3 {
public:
    std::string name;
    std::map<std::string, Skill2>  skills;
    void setORM(Mapper &mapper){
          mapper.orm("name", name);
          mapper.orm("skills", skills);
    }
};


TEST(JsonROM, child_child_not_exist_message){
     std::string message;
     try {
       std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
       boost::shared_ptr<Me3> me = JsonBind<Me3>().get(ifs);
     }  catch ( CppOrmException e) {
             message = e.what();
     }
     ASSERT_EQ(".skills.R.language_not_exist not found", message);
};
