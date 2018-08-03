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


class CSVBind {
private:
    bool ignore_unknown_filed;
public:
     boost::shared_ptr<CSVReader> csv;
     JsonBind json_binder;
public:
    CSVBind(){
        csv = boost::shared_ptr<CSVReader>(new CSVReader());
        ignore_unknown_filed = false;
    }
    void IgnoreUnknownKey() {
        this->ignore_unknown_filed = true;
        this->json_binder.IgnoreUnknownKey();
    }

    template<typename T>
    std::vector<T*> decode(std::istream& csv_content, std::istream*  header_stream = NULL){

        std::vector<T*> rc;
        if(header_stream != NULL) {
           csv->readRows(csv_content);
        }
        csv->readRows(csv_content);
        for(size_t i = 0; i < csv->rows.size(); i++) {
            Json::Value jv = this->createJsonObject(csv->headers,csv->rows[i].cells);
            rc.push_back(json_binder.decode2<T>(jv));
        }
        return rc;
    }


    template<typename T>
    void encodeToStream(std::vector<T*> data, std::ostream* output){

        std::vector<std::string> headers = this->csv->headers;

        //try get header from data
        if(headers.size() == 0 && data.size() != 0) {
            Json::Value jv = json_binder.encodeToJsonValue(*(data[0]));
            headers = json_binder.encoder.binder.encoded_key;
            
        } 
        appendCSVHeader(headers,output);
        for(size_t i = 0; i < data.size(); i++) {
            output[0] << "\n";
            Json::Value jv = json_binder.encodeToJsonValue(*(data[i]));
            this->appendCSVLine(headers,jv,output);
        }
    }

    template<typename T>
    std::string encodeToStr(std::vector<T*> data){
         std::stringstream ss;
         this->encodeToStream<T>(data, &ss);
         return ss.str();
    }

    template<typename T>
    void regTable(const std::map<std::string, T*> *table)
    {
        json_binder.regTable(table);
    }

    template<typename T>
    std::vector<T*> decode(const std::string& csv_content){
        std::stringstream ss;
        ss << csv_content;
        return this->decode<T>(ss);
    }
    
    template<typename T>
    std::vector<T*> decodeFile(std::string data_file, const char* header_file = NULL){
         std::vector<T*> rc;
         std::ifstream  fs(data_file.c_str());
         if(!fs) {
             printf("Can not open %s\n", data_file.c_str());
             assert("TODO, throw exception" == NULL);
         }
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
    Json::Value createJsonObject(const std::vector<std::string>& headers, const std::vector<std::string>& data){
        Json::Value jv;
        if(headers.size() != data.size() && (!this->ignore_unknown_filed)) {
            for(size_t i = 0; i < headers.size(); i++) {
                printf("header, %lu, [%s]\n", i, headers[i].c_str());
            }
            for(size_t i = 0; i < data.size(); i++) {
                printf("data, %lu, [%s]\n", i, data[i].c_str());
            }
            assert(false);
        }
        for(size_t i = 0; i < headers.size(); i++) {
             jv[headers[i]] = data[i];
             assert(jv[headers[i]].asString() == data[i]);
        }
        return jv;
    }

    void appendCSVHeader(const std::vector<std::string>& headers, std::ostream* output){
        for(size_t j = 0; j < headers.size(); j++) {
            const std::string& header = headers[j];
            if(j > 0) {
                     output[0] << ",";
            }
            output[0] << header;
         }
    }
    void appendCSVLine(const std::vector<std::string>& headers,const Json::Value &jv , std::ostream* output){
         std::stringstream csv_row;
         for(size_t j = 0; j < headers.size(); j++) {
             const std::string& header = headers[j];
             if(j > 0) {
                 csv_row << ",";
             }
             if(!jv.isMember(header)) {
                 std::stringstream ss;
                 Json::StyledStreamWriter writer;
                 writer.write(ss, jv);
                 printf("jv is: \n%s\n", ss.str().c_str());
                 assert(false);
             }
             std::string cell_str = jv[header].toStyledString();
             std::size_t x = cell_str.find_last_not_of("\r\n");
             if(x!= std::string::npos) {
                 cell_str = cell_str.substr(0, x+1);
             }
             if(jv[header].isString()) {
                 assert(cell_str.length() >= 2);
                 assert(cell_str[0] == '\"');
                 assert(cell_str[cell_str.length()-1] == '\"');
                 cell_str = cell_str.substr(1, cell_str.length()-2);
             }
             csv_row << cell_str;
         }
         output[0] << csv_row.str();
    }
    
};

}


#endif
