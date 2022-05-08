//
// Created by zck on 2022/5/2.
//

#ifndef CPPBIND_BINDERIMPBASE_H
#define CPPBIND_BINDERIMPBASE_H

#include "../foreign/ForeignTableMgmt.h"
namespace cppbind{
using namespace dynamic;
class EnconderDecoderBase : public cppbind::foreign::ForeignTableMgmt {
public:
    converter::StringConverterManager str_convert_mgmt;
    ClassRegisterBase* class_reg;
public:
    EnconderDecoderBase() {
        this->class_reg = NULL;
        str_convert_mgmt.addStringConverter<boost::posix_time::ptime,converter::BoostPTimeConverter>();
        str_convert_mgmt.addStringConverter<boost::posix_time::time_duration,converter::BoostTimeDurationConverter>();
        str_convert_mgmt.addStringConverter<boost::gregorian::date,converter::BoostGDateConverter>();
    }
    virtual ~EnconderDecoderBase() {}
public:
    void regClassRegister(ClassRegisterBase* _class_reg){
        assert(this->class_reg == NULL);
        this->class_reg = _class_reg;
    }

};
}
#endif //CPPBIND_TEST_BINDERIMPBASE_H
