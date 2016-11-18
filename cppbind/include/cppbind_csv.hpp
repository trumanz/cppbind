#ifndef __CPPBIND__CSV__
#define __CPPBIND__CSV__

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <list>
#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include "imp/binder.hpp"
#include "imp/csvcpp.h"
namespace  cppbind {

template<typename T>
class CSVBind{
public:
     CSVCPP csv_cpp;
public:
    //Decode string content
    std::vector<T> decode(const std::string& csv_content){
        std::stringstream ss;
        ss << csv_content;
        return this->decode(ss);
    }
    //Decoee istream content
    std::vector<T> decode(std::istream& csv_content){
        std::vector<T> es;
        std::vector<CSVRow> rows =  csv_cpp.get_rows(csv_content);
        for(std::vector<CSVRow>::iterator it = rows.begin(); it !=  rows.end(); it++) {
             T e = this->decode_row(*it);
             es.push_back(e);
        }
        return es;
    }
private:
    T decode_row(const CSVRow &row) const{
         T e;
         Binder binder(row.cells); 
         e.setBind(&binder);
         return e;
    }
};

}


#endif
