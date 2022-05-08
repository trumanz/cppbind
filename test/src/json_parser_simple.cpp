#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include "json/json.h"
#include <cppbind/JsonDecoder.hpp>
#include <cppbind/JsonEncoder.hpp>
#include <cppbind/csvbind.hpp>
#include "cppbind/object/ObjFactoryT.h"
#include <cppbind/converter/timestr.h>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <cppbind/util/EnumHelper.hpp>
#include "cppbind/dynamic/ClassRegister.h"

using namespace boost::posix_time;
using namespace cppbind;


class Student {
public:
    std::string name;
    int age;  
    std::string address;
    std::vector<std::string>  likes;
    boost::shared_ptr<boost::gregorian::date> born_date;
    void setBind(Binder *binder, bool load){
          binder->bind("name", name);
          binder->bind("age", age);
          binder->bind("likes", likes);
          binder->bind("bornDate", born_date);
          binder->bind("address", address, std::string("unknown"));
    }
    std::string toStr()const{
       std::stringstream ss;
       ss << "name:" << name;
       ss << ", age:" << age;
       ss << ",born: " << born_date;
       ss << ",address:" << address;
       ss << ", likes:";
       for (size_t i = 0 ; i < likes.size();i++) {
         if (i > 0) ss <<",";
         ss<< likes[i];
       }
       return ss.str();
    }
};

// https://en.cppreference.com/w/cpp/language/types
/*
TEST(parsr, fundamental_type_parse) {
    int x  = cppbind::decode_str("1")
}
 */

TEST(parse, primary_type){
    std::string json_str = "{\"name\" : \"Bruce\", \"age\": 18, \"likes\" : [\"football\", \"music\"] }";
    //decode
    Student* student = JsonDecoder().decode<Student>(json_str);
    printf("Decoded Student C++ Object:%s\n",  student->toStr().c_str());

    ASSERT_EQ(student->name, "Bruce");
    ASSERT_EQ(student->age, 18);
    ASSERT_EQ(student->likes.size(), 2);
    ASSERT_EQ(student->likes[0], "football");
    ASSERT_EQ(student->likes[1], "music");
    //encode
    std::string str = JsonEncoder().encodeToStr(*student);
    printf("Encoded Student Json String: %s\n", str.c_str());
    Json::Value jv;
    Json::Reader().parse(str, jv);
    ASSERT_EQ(jv["name"].asString(), "Bruce");
    ASSERT_EQ(jv["age"].asInt(), 18);
    ASSERT_EQ(jv["likes"][0].asString(), "football");
    ASSERT_EQ(jv["likes"][1].asString(), "music");
    delete student;
};


TEST(parse, simple){
  std::string json_str = "{\"name\" : \"Bruce\", \"age\": 18, \"likes\" : [\"football\", \"music\"] }";
  //decode



  Student* student = JsonDecoder().decode<Student>(json_str);
  printf("Decoded Student C++ Object:%s\n",  student->toStr().c_str());
  
  ASSERT_EQ(student->name, "Bruce");
  ASSERT_EQ(student->age, 18);
  ASSERT_EQ(student->likes.size(), 2);
  ASSERT_EQ(student->likes[0], "football");
  ASSERT_EQ(student->likes[1], "music");
  //encode
  std::string str = JsonEncoder().encodeToStr(*student);
  printf("Encoded Student Json String: %s\n", str.c_str());
  Json::Value jv;
  Json::Reader().parse(str, jv);
  ASSERT_EQ(jv["name"].asString(), "Bruce");
  ASSERT_EQ(jv["age"].asInt(), 18);
  ASSERT_EQ(jv["likes"][0].asString(), "football");
  ASSERT_EQ(jv["likes"][1].asString(), "music");
  delete student;
};
