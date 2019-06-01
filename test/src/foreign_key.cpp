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
    Customer* c = jb.decode<Customer>(jv.c_str());
    ASSERT_EQ(c->ht->name, "ShanDong");
};

class ForeginTable{
 public:
  map<string, Hometown*> foreign_table;
   Hometown* getObj(std::string key){
    if (foreign_table.count(key)) return foreign_table[key];
    foreign_table[key] = new Hometown(key);
    return foreign_table[key];
  }
};

TEST(foregin_key, decode_with_foregin_factory){
    cppbind::JsonBind jb;
    std::string jv = "{\"hometown\" : \"ShanDong\"} ";


    boost::shared_ptr<ForeginTable> ftable(new ForeginTable());
    jb.regTable<Hometown, ForeginTable>(ftable);
    Customer* c = jb.decode<Customer>(jv.c_str());
    ASSERT_EQ(c->ht->name, "ShanDong");
    ASSERT_EQ(ftable->foreign_table.size(), 1);
    ASSERT_EQ(ftable->foreign_table["ShanDong"]->name, "ShanDong");
};
