﻿#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <jsoncpp/json/json.h>
#include <cppbind/cppbind_json.hpp>
#include <cppbind/CSVBind.hpp>
#include <cppbind/ObjFactoryT.h>
#include <cppbind/type/timestr.h>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <cppbind/utility/EnumHelper.hpp>
#include <cppbind/ClassRegister.h>

using namespace boost::posix_time;
using namespace cppbind;
using namespace std;

class Hometown : public Object{
public:
   std::string name;
   Hometown(){}
   Hometown(const std::string& _name){this->name = _name;}
   std::string getKeyStr() const  { return name;}
   void setKeyStr(const std::string& _name){this->name = _name;}
};

class Customer {
public:
    Hometown* ht;
    void setBind(Binder *binder, bool load){
           binder->bindForeginKey("hometown", ht);
    }
};

TEST(foregin_key, decode){
    cppbind::JsonBind jb;
    std::string jv = "{\"hometown\" : \"ShanDong\"} ";
    map<string, Hometown*> foreign_table;
    foreign_table["ShanDong"] = new Hometown("ShanDong");
    jb.regTable((const map<string, Hometown*>*)&foreign_table);
    Customer* c = jb.decodeJsonString<Customer>(jv.c_str());
    ASSERT_EQ(c->ht->name, "ShanDong");
};

TEST(foregin_key, decode_with_foregin_factory){
    cppbind::JsonBind jb;
    std::string jv = "{\"hometown\" : \"ShanDong\"} ";
    map<string, Hometown*> foreign_table;
    jb.regTable(&foreign_table, boost::shared_ptr<ForeignObjFactory>(new ForeignObjFactoryT<Hometown>()));
    Customer* c = jb.decodeJsonString<Customer>(jv.c_str());
    ASSERT_EQ(c->ht->name, "ShanDong");
    ASSERT_EQ(foreign_table.size(), 1);
    ASSERT_EQ(foreign_table["ShanDong"]->name, "ShanDong");
};