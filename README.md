# CppBind



[![azure-pipeline](https://dev.azure.com/trumanz-dev/trumanz-github/_apis/build/status/trumanz.cppbind?branchName=master)](https://dev.azure.com/trumanz-dev/trumanz-github/_build/latest?definitionId=1&branchName=master)
[![sonar](https://sonarcloud.io/api/project_badges/measure?project=trumanz_cppbind&metric=alert_status)](https://sonarcloud.io/dashboard?id=trumanz_cppbind) 


CppBind is a C++ header-only library that allow mapping JSON/CSV values to/from C++ Class object. It's not a json/csv parser, it use [JsonCPP](https://github.com/open-source-parsers/jsoncpp) as low level parser. This project is inspried by Java [Jackson](https://github.com/FasterXML/jackson).


## Documentation
[CppBind documentation](http://trumanz.github.io/cppbind/) is generated using Doxygen.

## Using CppBind in your project
Just download and compile your project with cppbind;
```bash
git clone  https://github.com/trumanz/cppbind
g++  -I path/to/cppbind  example.cpp  -o  example
```

## Features
1. Data-binding C++ Class Object to and from JSON/CSV. Not oly 
2. Type of a Class member variable could be primitive type, other Class type or C++ standard container template(vector, list, map, set).
3. Class memeber variable could be  optional, predefined default value.
4. Foreigner table support. 
5. Dynamic type decode support.
6. User defined serialization and deserialization behavior.


## Why I write this library
I feel boring work when I am working on a C++ project which have plenty of configuration write by Json. I saw Java [Jackson](https://github.com/FasterXML/jackson) could easily mapping Json and Object, but C++ can not do that since it do not have reflection mechanism,  But C++ have template which let compiler auto generate useful codes by variable type.


## CppBind vs Bare JsonCpp

There is an simple example to show how cppbind simplfy codes. Here is the conclusion at beginning,   
| | CppBind |  Bare JsonCpp |
| :---- | :-----|:--------|
| Coding works | 5 lines | 40 lines |


Let's consider parse below JSON data to be A C++ Class Object.
JSON Data
```javascript
{
  "name" : "Bruce"
  "bornDate" : "2020-01-01",
  "skillsScore" : { 
     "football" : 8, 
      "music" : 7 
  }
}
```
With CppBind
```c++
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
  return JsonDecoder().decode<Student>(json_str);
}
```
Bare JsonCpp
```c++
class Person {
public:
   std::string name;
   boost::gregorian::date born_date;
   boost::shared_ptr< std::map<std::string, int> > optional_skills_score;
}
Person* parsePerson(std::istream &json_input)
{
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(json_input, root);
    Person *p = new Person();
    //parse name
    Json::Value jv_name = root["name"];
    if (!jv_name.isNull()) {
        p->name = jv_name.asString()
    } else {
        delete p;
        return NULL;
    }
    //parse born date, this code missing data pattern check
    Json::Value jv_born_date = root["bornDate"]
    if (!jv_name.isNull()) {
        std::istringstream ss(jv_name.asString());
        boost::gregorian::date d;
        ss >> d;
        p->born_date.reset(new boost::gregorian::date(d));
    }
    //parse optonal skill score map
    Json::Value jv_skill_score = root["skillsScore"]
    if (!skill_score.isNull()) {
        if (!jv_skill_score.isObject()) {
            delete p;
            return NULL;
        }
        p->optional_skills_score.reset(new std::map<std::string, int>());
        Json::Value::Members keys = jv_skill_score.getMemberNames();
        for (Json::Value::Members::iterator it = keys.begin(); 
            it != keys.end(); it++) {

            std::string key_str = *it;
            Json::Value jv_value = jv_skill_score[*it];
            int score = jv_value.asInt();
            p->optional_skills_score->insert({ key_str, score });

        }
    }
    return p;
}
```

## Documentation
### Sorry！ I'm still working for this document, below are some draft.
[trumanz.github.io/cppbind/](https://trumanz.github.io/cppbind/)


### Quick start


### API

示例代码



#### 1. setBind::bind函数

示例代码

setBind函数：自定义类Student，并实现成员函数setBind， 函数实现为为调用binder->bind将json node名称值与成员变量建立联系；

JsonDecoder().decode模板函数：解码一个json string为一个C++类对象。

JsonDecoder().encodeToStr模板函数：编码一个C++类对象为一个json string。

binder默认支持绑定的基本类型：

|                                  | 说明                          |
| :------------------------------- | ----------------------------- |
| bool                             |                               |
| int32_t                          |                               |
| uint32_t                         |                               |
| int64_t                          |                               |
| uint64_t                         |                               |
| float                            |                               |
| double                           |                               |
| std::string                      |                               |
| Json::Value                      |                               |
| boost::posix_time::ptime         | 默认格式"YYYY-mm-dd HH:MM:SS" |
| boost::gregorian::date           | 默认格式"YYYY-mm-dd"          |
| boost::posix_time::time_duration | 默认格式"HH:MM:SS”            |

binder支持的C++模板类型

|             | 说明                                                         |
| ----------- | ------------------------------------------------------------ |
| std::vector | 元素可为对象或对象指针,  映射为json array                    |
| std::list   | 元素可为对象或对象指针,  映射为json array                    |
| std::set    | 元素可为对象或对象指针,  映射为json array                    |
| std::map    | key值为string类型，元素可为对象或对象指针，映射为json object |

 模板对象之间可相互嵌套，如下

```c++
class MyObj{
   std::map<std::string, std::vector<Student> > group_students;
   std::set<std::set<Student> >  group_students_v2;
   void setBind(Binder *binder, bool load){
          binder->bind("group_students", group_students);
          binder->bind("group_students_v2", group_students_v2);
    }
}
```



#### 2. 外部类型支持（无setBind函数）

```
参考  StringConverterManager str_convert_mgmt;
可以通过向 binder中注册外部类型的Converter实现；默认支持的对外部boost::posix_time::ptime， boost::gregorian::date， boost::posix_time::time_duration即通过此方式
```

#### 3.bindForeginKey绑定对象指针指向外部对象

```

```
## Development
### macos

refer devops/build-ubuntu-vcpkg.yml


CI refered by 
https://github.com/microsoft/cpprestsdk/blob/master/azure-pipelines.yml
