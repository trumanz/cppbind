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
#include <boost/algorithm/string.hpp>
#include "imp/binder.hpp"

namespace  cppbind {

template<typename T>
class CSVBind{
public:
    std::string  delimeters;
    bool continues_delimter_as_one_delimter;
    std::string line_break;
    bool strip_white_space;
public:
    CSVBind(){
        delimeters = ",";
        continues_delimter_as_one_delimter = false;
        line_break = "\n";
        strip_white_space = true;
    }
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
            T e = this->decode_line(line);
            es.push_back(e);
        }
        return es;
    }

private:
    T decode_line(const std::string &line) const{
         T e;
         std::vector<std::string> cells = this->get_cells(line);
         Binder binder(cells); 
         e.setBind(&binder);
         return e;
    }

    std::vector<std::string> get_cells(const std::string &line) const{
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
            cells.push_back(cell);
            
        }
        return cells;
    }
};

}


#endif
