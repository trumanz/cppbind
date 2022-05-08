//
// Created by zck on 2022/5/8.
//

#ifndef CPPBIND_TEST_CLASSMISSREGEXCEPTION_H
#define CPPBIND_TEST_CLASSMISSREGEXCEPTION_H
namespace  cppbind { namespace exception {


class ClassMissRegException : public ParseErrorException {
public:
    ClassMissRegException(std::string class_name) :
            ParseErrorException(class_name + " miss register"){
    }
};

} }
#endif //CPPBIND_TEST_CLASSMISSREGEXCEPTION_H
