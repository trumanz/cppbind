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
#include "type/strtime.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "cppbind_spec_type.hpp"

using namespace boost::posix_time;
using namespace cppbind;


class PosixTime : public ptime, public SpecTypeBase {
public:
      PosixTime(){
          this->pattern = "%Y-%m-%d %H:%M:%S";
      }
private:
      virtual int decode(const std::string &str, std::string *errmsg) {
        //printf("decode %s\n", str.c_str());
        StrTime strTime(this->pattern.c_str());
        ptime pt;
        int ret = strTime.parser(str, &pt);
        //printf("ret = %d\n", ret);
        if(ret != 0) {
            errmsg[0] += " can not parser [" + str + "]"  + " with pattern "  + pattern;
            return -1;
        } else {
            ptime *this_pt = dynamic_cast<ptime*>(this);
            this_pt[0] = pt;
            return 0;
        }
    }
    virtual std::string encode() {
        StrTime strTime(this->pattern.c_str());
        return strTime.format(*this);
    }
    std::string pattern;
};

class Contact {
public:
    Contact(){}
    std::string email;
    std::string phone;
    void setBind(Mapper &mapper){
          mapper.bind("email", email);
          mapper.bind("phone", phone);
    }

};

class Skill {
public:
    int grade;
    void setBind(Mapper &mapper) {
          mapper.bind("grade", grade);
    }
};

class Me {
public:
    std::string name; //basic type
    int age;  //
    PosixTime born;
    Contact contact; // class object
    std::list<std::string>  likes;  // std list
    std::map<std::string, Skill>  skills; // class list
    boost::shared_ptr<int>  value_not_exist; //optional not exist
    void setBind(Mapper &mapper){
          mapper.bind("name", name);
          mapper.bind("age", age);
          mapper.bind("born", born);
          mapper.bind("contact", contact);
          mapper.bind("likes", likes);
          mapper.bind("skills", skills);
          mapper.bind("value_not_exist",  value_not_exist);
    }
};

TEST(JsonROM, baisc){

     std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
     boost::shared_ptr<Me> me = JsonBind<Me>().decode(ifs);
     //basic type
     ASSERT_EQ(me->name ,"truman");
     ASSERT_EQ(me->age ,30);
     StrTime tf("%Y-%m-%d %H:%M:%S");
     printf("born: %s\n",  tf.format(me->born).c_str());

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
