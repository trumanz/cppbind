#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include "cppbind_csv.hpp"

using namespace cppbind;
class Contact {
public:
    Contact(){}
    std::string email;
    std::string phone;
    void setBind(Binder *binder){
          binder->bind("email", email);
          binder->bind("phone", phone);
    }
};

class Person {
public:
    std::string name; //basic type
    int age;  //
    Contact contact; // class object
    void setBind(Binder *binder){
          binder->bind("name", name);
          binder->bind("age", age);
          binder->bind("contact", contact);
    }
};

 
TEST(CSV_Parser, baisc){
    std::string csv_content;
    csv_content +=    "John, 20, John@test.com, 1234";
    csv_content += "\nAnny, 23, Anny@test.com, 5678";

    std::vector<Person> rs = CSVBind<Person>().decode(csv_content);
    ASSERT_EQ(rs.size(), 2);
    Person p = rs[0];
    ASSERT_EQ(p.name, "John");
    ASSERT_EQ(p.age, 20);
    ASSERT_EQ(p.contact.email, "John@test.com");
    ASSERT_EQ(p.contact.phone, "1234");

    p = rs[1];
    ASSERT_EQ(p.name, "Anny");
    ASSERT_EQ(p.age, 23);
    ASSERT_EQ(p.contact.email, "Anny@test.com");
    ASSERT_EQ(p.contact.phone, "5678");

};
