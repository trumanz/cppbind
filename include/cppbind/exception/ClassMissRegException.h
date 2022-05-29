//
// Created by zck on 2022/5/8.
//

#ifndef CPPBIND_TEST_CLASSMISSREGEXCEPTION_H
#define CPPBIND_TEST_CLASSMISSREGEXCEPTION_H
namespace  cppbind { namespace exception {


class ClassMissRegException : public ParseErrorException {
public:
    explicit ClassMissRegException(std::string class_name) noexcept :
            ParseErrorException(class_name + " miss register")  {
    }
    //ClassMissRegException(const ClassMissRegException& obj) noexcept = default;

};

} }
#endif //CPPBIND_TEST_CLASSMISSREGEXCEPTION_H
