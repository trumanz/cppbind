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
#include "cppbind_json.hpp"
#include "imp/CSVReader.h"
namespace  cppbind {


class CSVBind{
public:
     CSVReader csv;
public:
    CSVBind(){}

    template<typename T>
    std::vector<T*> decode(std::istream& csv_content, std::istream*  header_stream = NULL){

        std::vector<T*> rc;
        if(header_stream != NULL) {
           csv.readRows(csv_content);
        }
        csv.readRows(csv_content);
        for(size_t i = 0; i < csv.rows.size(); i++) {
            Json::Value jv = this->createJsonObject(csv.headers,csv.rows[i].cells);
            rc.push_back(JsonBind().decode2<T>(jv, true));
        }
        return rc;
    }

    template<typename T>
    std::vector<T*> decode(const std::string& csv_content){
        std::stringstream ss;
        ss << csv_content;
        return this->decode<T>(ss);
    }
    
    template<typename T>
    std::vector<T*> decodeFile(const char *data_file, const char* header_file = NULL){
         std::vector<T*> rc;
         std::ifstream  fs(data_file);
         if(!fs)  assert("TODO, throw exception" == NULL);
         if(header_file != NULL) {
             std::ifstream  fs_header(header_file);
             if(!fs_header)  assert("TODO, throw exception" == NULL);
             rc = this->decode<T>(fs, &fs_header);
         } else {
             rc = this->decode<T>(fs, NULL);
         }
         fs.close();
         return rc;
    }
private:
    template<typename T>
    T* decodeRow(const CSVRow &row) const{
         T *e;
         CSVDecodeBinder decoder(row.cells);
         decoder.decode(e);
         return e;
    }

    Json::Value createJsonObject(const std::vector<std::string>& headers, const std::vector<std::string>& data){
        Json::Value jv;
        if(headers.size() != data.size()) {
            for(size_t i = 0; i < headers.size(); i++) {
                printf("header, %lu, %s\n", i, headers[i].c_str());
            }
            for(size_t i = 0; i < data.size(); i++) {
                printf("data, %lu, %s\n", i, data[i].c_str());
            }
        }
        assert(headers.size() == data.size());
        for(size_t i = 0; i < headers.size(); i++) {
            jv[headers[i]] = data[i];
        }
        return jv;
    }
};

}


#endif
