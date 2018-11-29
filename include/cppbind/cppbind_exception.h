#ifndef __CPPBIND__EXCEPTION_H__
#define __CPPBIND__EXCEPTION_H__
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdexcept>
#include <stdint.h>


namespace  cppbind {


class ParseErrorException : public std::exception
{
private:
    std::vector<std::string> node_info;
    std::string parse_error_msg;
    std::string error_str_buf;
    void buildErrorStr(){
        error_str_buf = "";
        for(size_t i = 0; i < this->node_info.size(); i++) {
            if(i > 0) error_str_buf += ".";
            error_str_buf += node_info[i];
        }
        error_str_buf += " " + this->parse_error_msg;
    }
public:
    virtual ~ParseErrorException() throw (){}
    ParseErrorException(std::string const& node_name, const std::string err_msg)
    {
        this->node_info.insert(this->node_info.begin(), node_name);
        this->parse_error_msg = err_msg;
        this->buildErrorStr();
    }
    
    ParseErrorException(const std::string err_msg)
    {
        this->parse_error_msg = err_msg;
        this->buildErrorStr();
    }
    //ParseErrorException(const ParseErrorException& e, std::string const& node_name)
    //{
    //    this->node_info = e.node_info;
    //    this->parse_error_msg = e.parse_error_msg;
    //    this->node_info.insert(this->node_info.begin(), node_name);
    //    this->buildErrorStr();
    //}
    void addParentNodeName(std::string const& node_name) {
        this->node_info.insert(this->node_info.begin(), node_name);
        this->buildErrorStr();
    }
    
    const char* what() const throw() {
        return this->error_str_buf.c_str();
    }
    
};

class ClassMissRegException : public ParseErrorException {
public:
    ClassMissRegException(std::string class_name) : 
        ParseErrorException(class_name + " miss register"){
    }
};

}

#endif
