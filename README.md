# cppbind

| build | status                                                                                        |
|-------|-----------------------------------------------------------------------------------------------|
| Travis-CI   | ![alt text](https://travis-ci.org/trumanz/cpp-learning.svg?branch=master "Logo Title Text 1") |
| Azure-Pipelines   | [![Build Status](https://dev.azure.com/trumanckzhou/trumanckzhou/_apis/build/status/trumanz.cppbind?branchName=master)](https://dev.azure.com/trumanckzhou/trumanckzhou/_build/latest?definitionId=1&branchName=master) |

## A library for conversion between c++ class object and JSON string
### Feature
1. Serialize a c++ object to JSON string.
2. Deserialize a JSON string to c++ object.
3. Support optional node when serialize and deserialize.
4. Support nested Class object.
5. Support c++ standard tempalte Vector, List and Map.
6. Support user defined deserialzie and deserialize.


## Documentation

[trumanz.github.io/cppbind/](https://trumanz.github.io/cppbind/)



### 快速入门



### API

示例代码



#### 1. setBind::bind函数

示例代码

```c++

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

int main(){
  std::string json_str = "{\"name\" : \"Bruce\", \"age\": 18, \"likes\" : [\"football\", \"music\"] }";
  //decode
  Student* student = JsonBind().decode<Student>(json_str);
  printf("Decoded student:%s\n",  student->toStr().c_str());
  //encode
  std::string str = JsonBind().encodeToStr(*student);
  printf("Encoded Student Json String: %s\n", str.c_str());
  delete student;
}
```

运行结果：

```shell
Decoded Student C++ Object:name:Bruce, age:18,born: 0,address:unknown, likes:football,music
Encoded Student Json String: {
        "address" : "unknown",
        "age" : 18,
        "likes" : [ "football", "music" ],
        "name" : "Bruce"
}
```

setBind函数：自定义类Student，并实现成员函数setBind， 函数实现为为调用binder->bind将json node名称值与成员变量建立联系；

JsonBind().decode模板函数：解码一个json string为一个C++类对象。

JsonBind().encodeToStr模板函数：编码一个C++类对象为一个json string。

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

```

```
