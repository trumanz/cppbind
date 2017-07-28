# cppbind

| build | status                                                                                        |
|-------|-----------------------------------------------------------------------------------------------|
| Travis-CI   | ![alt text](https://travis-ci.org/trumanz/cpp-learning.svg?branch=master "Logo Title Text 1") |

## A library for conversion between c++ class object and JSON string
### Feature
1. Serialize a c++ object to JSON string.
2. Deserialize a JSON string to c++ object.
3. Support optional node when serialize and deserialize.
4. Support nested Class object.
5. Support c++ standard tempalte Vector, List and Map.
6. Support user defined deserialzie and deserialize.

### Quick start
1. define your Class with member funciton setBind.
setBind funciton should call binder->bind for build relationship with the class member and it's name.
```
class Skill {
public:
    int grade;
    void setBind(Binder *binder) {
          binder->bind("grade", grade);
    }
};
class Me {
public:
    std::string name; //basic type
    int age;  //
    PosixTime born;
    Contact contact; // class object
    std::list<std::string>  likes;  // std list
    std::vector<std::string>  hates;  // std vector
    std::map<std::string, Skill>  skills; // std map
    boost::shared_ptr<int>  value_not_exist; //optional not exist
    void setBind(Binder *binder){
          binder->bind("name", name);
          binder->bind("age", age);
          binder->bind("born", born);
          binder->bind("contact", contact);
          binder->bind("likes", likes);
          binder->bind("hates", hates);
          binder->bind("skills", skills);
          binder->bind("value_not_exist",  value_not_exist);
    }
};
```

2. Serialize a object to a stream

```
Me me;
std::stringstream ss;

```

3. Deserialize a stream to object

```
Me me;
...
std::stringstream ss;
JsonBind<Me>  jbind;
jbind.encode(me, &ss);
printf("%s\n", ss.str().c_str());
```

```
std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
boost::shared_ptr<Me> me = JsonBind<Me>().decode(ifs);
```
