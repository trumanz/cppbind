#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <jsoncpp/json/json.h>
#include <cppbind/jsonbind.hpp>
#include <cppbind/csvbind.hpp>
#include <cppbind/ObjFactoryT.h>
#include <cppbind/type/timestr.h>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <cppbind/utility/EnumHelper.hpp>
#include <cppbind/ClassRegister.h>

using namespace boost::posix_time;
using namespace cppbind;

#if 0
class PosixTime : public ptime, public SpecTypeBase {
public:
      PosixTime(){
          this->pattern = "%Y-%m-%d %H:%M:%S";
      }
private:
      virtual int decode(const std::string &str, std::string *errmsg) {
        //printf("decode %s\n", str.c_str());
        TimeStr strTime(this->pattern.c_str());
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
        TimeStr strTime(this->pattern.c_str());
        return strTime.format(*this);
    }
    std::string pattern;
};
#endif

class SexEnum{
public:
   enum Type {
       MALE,
       FEMALE
   };
   static const char* enum_str_info[];
   static const size_t enum_str_info_len;
};

const char* SexEnum::enum_str_info[] = {"male", "female"};
const size_t SexEnum::enum_str_info_len = sizeof(SexEnum::enum_str_info)/sizeof(SexEnum::enum_str_info[0]);

typedef EnumHelper<SexEnum> SexType;

class Vehicle : public Object{
public:
    virtual ~Vehicle(){}

};
class Metro : public Vehicle{

public:
    class Data4Bind{
    public:
        int line;
         void setBind(Binder *binder, bool load){
             binder->bind("line", line);
          }
    };
    Metro(const Data4Bind& d){this->data = d;}
    const Data4Bind& getData4Bind() const { return data;}
public:
    Data4Bind data;
};

class Music {
public:
    const std::string& getKeyStr(){
        return this->id;
    }
    std::string id;
    std::string name;
    std::string singer;
    float grade;
    void setBind(Binder *binder, bool load){
          binder->bind("id", id);
          binder->bind("name", name);
          binder->bind("singer",singer);
          binder->bind("grade",grade);
    }

};


class Contact {
public:
    Contact(){}
    std::string email;
    std::string phone;
    void setBind(Binder *binder, bool load){
          binder->bind("email", email);
          binder->bind("phone", phone);
    }

};

class Skill {
public:
    int grade;
    void setBind(Binder *binder, bool load) {
          binder->bind("grade", grade);
    }
};

class Me {
public:
    std::string name; //basic type
    int age;  //
    SexType sex;
    ptime born;
    boost::gregorian::date born_date;
    time_duration  born_time_of_day;
    Contact contact; // class object
    std::list<std::string>  likes;  // std list
    std::vector<std::string>  hates;  // std vector
    std::vector<std::string>  work_experience;  // std vector
    std::map<std::string, Skill>  skills; // std map
    boost::shared_ptr<int>  value_not_exist; //optional not exist
    Json::Value jv; //optional not exist
    std::vector<Music*> liked_music_list;
    Music* favorite_music;
    Vehicle* vehicle;
    std::map<SexType, std::string> favorite_singer;
    void setBind(Binder *binder, bool load){
          binder->bind("name", name);
          binder->bind("age", age);
          binder->bind("sex", sex);
          binder->bind("born", born);
          binder->bind("born_date", born_date);
          binder->bind("born_time_of_day", born_time_of_day);
          binder->bind("contact", contact);
          binder->bind("likes", likes);
          binder->bind("hates", hates);
          binder->bind("work_experience", work_experience);
          binder->bind("skills", skills);
          binder->bind("value_not_exist",  value_not_exist);
          binder->bind("jv", jv);
          binder->bindForeginKey("liked_music", liked_music_list);
          binder->bindForeginKey("favorite_music", favorite_music);
          binder->bindDynamicType("vehicle", vehicle);
          binder->bind("favorite_singer", favorite_singer);
    }
};

TEST(JsonROM, baisc){

     std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
     std::ifstream music_csv("./sample_data/music.csv",  std::ifstream::in);

     cppbind::CSVBind music_csv_binder;
     std::vector<Music*> all_musics =music_csv_binder.decode<Music>(music_csv);
     std::map<std::string, Music*> music_table;
     for(size_t i = 0; i < all_musics.size(); i++) {
          music_table[all_musics[i]->id] = all_musics[i];
     }
     ASSERT_EQ(music_table["music_1"]->name, "Dirty Diana");

     std::string csv_str = cppbind::CSVBind().encodeToStr<Music>(all_musics);
     printf("csv_str=\n[%s]\n", csv_str.c_str());

     printf("all_music.json = ^ %s ^\n", JsonBind().toJsonStr(all_musics).c_str());


     ClassRegister class_register;
     class_register.regClass<Metro>("Metro");

     JsonBind binder;
     binder.regTable(&music_table);
     binder.regClassRegister(&class_register);

     boost::shared_ptr<Me> me(binder.decode<Me>(ifs));

     printf("decode ME sucess\n");
     //basic type
     ASSERT_EQ(me->name ,"truman");
     ASSERT_EQ(me->age ,30);
     ASSERT_EQ(me->sex.value , SexEnum::MALE );
     TimeStr tf("%Y-%m-%d %H:%M:%S");
     printf("born: %s\n",  tf.format(me->born).c_str());

     //optional 
     ASSERT_EQ(me->value_not_exist.get() , (int*)NULL);

     //std string list
     std::list<std::string>  likes = me->likes;
     ASSERT_EQ(likes.size(), 2);
     ASSERT_EQ(likes.front(), "Batman");
     ASSERT_EQ(likes.back(), "Superman");

     std::vector<std::string>  hates = me->hates;
     ASSERT_EQ(hates.size(), 2);
     ASSERT_EQ(hates[0], "war");
     ASSERT_EQ(hates[1], "dictator");
     

      //  class  object
     Contact contact = me->contact;
     ASSERT_EQ(contact.email, "@com");
     ASSERT_EQ(contact.phone, "123456");

     // std map 
     std::map<std::string,  Skill>  skills = me->skills;
     ASSERT_EQ(skills.size(), 2);
     ASSERT_EQ(skills["c++"].grade, 7);
     ASSERT_EQ(skills["R"].grade, 0);

     //foreginkey
     ASSERT_EQ(me->favorite_music, music_table.find("music_0")->second);
     ASSERT_EQ(me->liked_music_list.size(), 2);
     ASSERT_EQ(me->liked_music_list[0], music_table.find("music_1")->second);
     ASSERT_EQ(me->liked_music_list[1], music_table.find("music_2")->second);

     //dynamicType
     Metro* metro = dynamic_cast<Metro*>(me->vehicle);
     ASSERT_TRUE(metro != NULL);
     ASSERT_EQ(3, metro->data.line);

     for(int i = 0; i < 10; i++) {
         std::stringstream ss;
         binder.encode(*(me.get()), &ss);
     }
     std::stringstream ss;
     binder.encode(*(me.get()), &ss);
     printf("%s\n", ss.str().c_str());


     for(size_t i = 0; i < all_musics.size(); i++) {
         delete all_musics[i];
     }
};
