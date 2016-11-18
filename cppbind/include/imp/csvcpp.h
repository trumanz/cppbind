#ifndef __CPPBIND__CSV_H__
#define __CPPBIND__CSV_H__

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <list>
#include <boost/shared_ptr.hpp>
#include <stdexcept>
  
namespace  cppbind {

class CSVRow{
public:
    std::vector<std::string> cells;
};

class CSVCPP{
public:
    std::string  delimeters;
    bool continues_delimter_as_one_delimter;
    std::string line_break;
    bool strip_white_space;
public:
    CSVCPP();
public:
     std::vector<CSVRow> get_rows(std::istream& csv_content) const;
private:
    CSVRow get_row(const std::string &line) const;

};

}


#endif
