CppBind                         {#mainpage}
============

@section Intro Introduction


cppbind  introduction

 
@section features Features
                          

1. Data-binding C++ Class Object to and from JSON/CSV. Not oly 
2. Type of a Class member variable could be primitive type, other Class type or C++ standard container template(vector, list, map, set).
3. Class memeber variable could be  optional, predefined default value.
4. Foreigner table support. 
5. Dynamic type decode support.
6. User defined serialization and deserialization behavior.


@section codes_example Code example 

~~~~~~~~~~~~~{.cpp}
class Person {
public:
    std::string name;
    boost::gregorian::date born_date;
    boost::shared_ptr< std::map<std::string, int> > optional_skills_score;
    void setBind(Binder *binder, bool load){
          binder->bind("name", name);
          binder->bind("bornDate", born_date);
          binder->bind("skillsScore", optional_skills_score);
    }
};
Person* parsePerson(std::istream &json_input){
  return JsonBind().decode<Student>(json_str);
}
~~~~~~~~~~~~~


