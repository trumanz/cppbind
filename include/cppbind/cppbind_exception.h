#ifndef __CPPBIND__EXCEPTION_H__
#define __CPPBIND__EXCEPTION_H__
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdexcept>
#include <stdint.h>


namespace  cppbind {


class CppBindException : public std::runtime_error
{
   public:
    CppBindException(std::string const& node_name, const std::string base_msg)
        : runtime_error(node_name + " "  + base_msg)
    { }
    
    CppBindException(const std::string base_msg)
        : runtime_error(std::string(" ")  + base_msg)
    { }
    CppBindException(const CppBindException& e, std::string const& node_name)
        : runtime_error(node_name + e.what())

    { }
    
};

class ClassMissRegException : public std::runtime_error{
public:
    ClassMissRegException(std::string class_name) : 
        runtime_error(class_name + " miss register"){

    }
};

}

#endif
