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
#include <boost/algorithm/string.hpp>
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
    CSVCPP(){
    delimeters = ",";
    continues_delimter_as_one_delimter = false;
    line_break = "\n";
    strip_white_space = true;
}
public:
   virtual std::vector<CSVRow> get_rows(std::istream& csv_content) const{
    std::vector<CSVRow> rs;
    std::string  line;
    while(true) {
        if(csv_content.eof()) {
            break;
        }
        if(csv_content.fail()) {
            assert("TODO" == NULL);
        }
        //http://www.cplusplus.com/reference/string/string/getline/
        std::getline(csv_content, line);
        rs.push_back(get_row(line));
    }
    return rs;
}

protected:
    virtual CSVRow get_row(const std::string &line) const{
    CSVRow row;
    //printf("====%s\n", line.c_str());
    std::vector<std::string> cells;
    std::string cell;
    std::string remains = line;
    while(!remains.empty()) {
       //printf("remains====%s\n", remains.c_str());
        size_t pos = remains.find_first_of(delimeters);
        if(pos != std::string::npos) {
            cell  = remains.substr(0, pos);
            remains = remains.substr(pos+1);
        } else{
            cell = remains;
            remains = "";
        }
        if(strip_white_space) {
            boost::trim(cell);
        }
        //printf("cell====[%s]\n", cell.c_str());
        row.cells.push_back(cell);

    }
    return row;
}


};

}


#endif
