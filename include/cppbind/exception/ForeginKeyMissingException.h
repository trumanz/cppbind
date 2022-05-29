//
// Created by zck on 2022/5/8.
//

#ifndef CPPBIND_TEST_FOREGINKEYMISSINGEXCEPTION_H
#define CPPBIND_TEST_FOREGINKEYMISSINGEXCEPTION_H
namespace  cppbind { namespace exception {

class ForeginKeyMissingException : public ParseErrorException {
public:
    ForeginKeyMissingException(std::string obj_name) noexcept :
    ParseErrorException(obj_name + " foregin key missing") {
    }

    //ForeginKeyMissingException(const ForeginKeyMissingException& obj) noexcept = default;
};

}}
#endif //CPPBIND_TEST_FOREGINKEYMISSINGEXCEPTION_H
