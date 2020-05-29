#ifndef __CPPBIND__CSV_READER_H__
#define __CPPBIND__CSV_READER_H__

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
#include <boost/tokenizer.hpp>
#include <stdexcept>
  
namespace  cppbind {

class CSVRow{
public:
    std::vector<std::string> cells;
    unsigned int line_no;
    std::string original_line;
};

class CSVReader{
private:
    const std::string dropped_delims;
    const bool trim;
public:
    std::vector<std::string> headers;
    std::vector<CSVRow> rows;
    virtual ~CSVReader(){}
    CSVReader(const std::string _dropped_delims=",", bool _trim = false) : dropped_delims(_dropped_delims), trim(_trim){}
public:
   void readFile(const char *filename){
       std::ifstream _in(filename);
       readRows(_in);
       _in.close();
   }
   virtual void readRows(std::istream& csv_content) {
      unsigned int line_no = 0;
      while(!csv_content.eof()) {
         std::string  line;
         if(csv_content.fail()) assert("TODO" == NULL);
         std::getline(csv_content, line);
         line_no++;
         if(line.length() == 0) continue;
         CSVRow row = getRow(line, line_no);
         if(this->headers.size() == 0) {
             this->headers = row.cells;
         } else {
             rows.push_back(row);
         }
     }
   }
protected:
    virtual CSVRow getRow(const std::string &line, unsigned int line_no) const{
    CSVRow row;
    row.line_no = line_no;
    row.original_line = line;
    boost::char_separator<char> sep(this->dropped_delims.c_str(), "", boost::keep_empty_tokens);
    boost::tokenizer< boost::char_separator<char> > tok(line,sep);
    for (auto it = tok.begin(); it != tok.end(); it++) {
        if (this->trim) {
            row.cells.push_back( boost::algorithm::trim_copy(*it) );
        } else {
            row.cells.push_back(*it);
        }
    }
    return row;
}


};

}


#endif
