#ifndef __CPPBIND__EXCEPTION_H__
#define __CPPBIND__EXCEPTION_H__
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdexcept>
#include <stdint.h>
#include "ParseErrorException.h"
#include "ClassMissRegException.h"
#include "ForeginKeyMissingException.h"

namespace  cppbind { namespace exception {

class Util {
public:
    template<typename  T>
    static void throwExeption(T& e){
        throw e;
    }
};


} }

#endif
