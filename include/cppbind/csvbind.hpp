﻿#ifndef __CPPBIND__CSV__
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
#include "JsonDecoder.hpp"
#include "JsonEncoder.hpp"
#include "imp/CSVReader.h"
namespace  cppbind {


class CSVBind {
private:
    bool ignore_unknown_filed;
public:
     boost::shared_ptr<CSVReader> csv;
     JsonDecoder json_decoder;
     JsonEncoder json_encoder;
public:
    CSVBind(const std::string _dropped_delims=",", bool _trim = false){
        csv = boost::shared_ptr<CSVReader>(new CSVReader(_dropped_delims, _trim));
        ignore_unknown_filed = false;
    }
    void IgnoreUnknownKey() {
        this->ignore_unknown_filed = true;
        this->json_decoder.ignoreUnknownNode();
    }

    template<typename T>
    std::vector<T*> decode(std::istream& csv_content, std::istream*  header_stream = nullptr,
                            const std::vector<std::string> *default_value = nullptr){

        std::vector<T*> rc;
        if(header_stream != nullptr) {
           csv->readRows(csv_content);
        }
        csv->readRows(csv_content);
        for(size_t i = 0; i < csv->rows.size(); i++) {
            Json::Value jv = this->createJsonObject(csv->headers,csv->rows[i].cells, default_value);
            auto e = json_decoder.decode<T>(jv);
            rc.push_back(e.get());
            e.release();
        }
        return rc;
    }


    template<typename T>
    void encodeToStream(std::vector<T*> data, std::ostream* output){

        std::vector<std::string> headers = this->csv->headers;

        //try get header from data
        if(headers.size() == 0 && data.size() != 0) {
            Json::Value jv = json_encoder.encode(*(data[0]));
            headers = json_encoder.encoder.binder.encoded_key;
        } 
        appendCSVHeader(headers,output);
        for(size_t i = 0; i < data.size(); i++) {
            output[0] << "\n";
            Json::Value jv = json_encoder.encode(*(data[i]));
            this->appendCSVLine(headers,jv,output);
        }
    }

    template<typename T>
    std::string encodeToStr(std::vector<T*> data){
         std::stringstream ss;
         this->encodeToStream<T>(data, &ss);
         return ss.str();
    }

    template<typename ObjT>
    void regTable(const std::map<std::string, ObjT*> *table)
    {   
        json_decoder.regTable(table);
    } 
    template<typename ObjT, typename FTable>
    void regTable(boost::shared_ptr<FTable> ft){
        json_decoder.regTable<ObjT, FTable>(ft);
    }

    template<typename T>
    std::vector<T*> decode(const std::string& csv_content) {
        std::stringstream ss;
        ss << csv_content;
        return this->decode<T>(ss);
    }
    
    template<typename T>
    std::vector<T*> decodeFile(std::string data_file, const char* header_file = nullptr,
                               const std::vector<std::string> *default_value = nullptr){
         std::vector<T*> rc;
         std::ifstream  fs(data_file.c_str());
         if(!fs) {
             printf("Error, Can not open %s\n", data_file.c_str());
             assert("TODO, throw exception" == nullptr);
         }
         if(header_file != nullptr) {
             std::ifstream  fs_header(header_file);
             if(!fs_header)  assert("TODO, throw exception" == nullptr);
             rc = this->decode<T>(fs, &fs_header, default_value);
         } else {
             rc = this->decode<T>(fs, nullptr, default_value);
         }
         fs.close();
         return rc;
    }
private:
    Json::Value createJsonObject(const std::vector<std::string>& headers, const std::vector<std::string>& data,
                                 const std::vector<std::string> *default_value = nullptr){
        Json::Value jv;

        bool data_not_align = false;
        if (data.size() + (default_value ? default_value->size() : 0) < headers.size()) {
            data_not_align = true;
        }
        if (data.size() > headers.size() && (!this->ignore_unknown_filed)) {
            data_not_align = true;
        }
        
        if(data_not_align) {
            const size_t max_num = headers.size() > data.size() ?  headers.size() : data.size();
            for(size_t i = 0; i < max_num; i++) {
                std::string h = i < headers.size() ? headers[i] : std::string("");
                std::string d = i < data.size() ? data[i] : std::string("");
                printf("%zu, [%s] : [%s]\n", i, h.c_str(), d.c_str());
            }
            assert(false);
        }
        for(size_t i = 0; i < headers.size(); i++) {
            std::string d = i < data.size() ? data[i] : default_value->at(default_value->size() - (headers.size() -i));
            if(d.length()> 0)   jv[headers[i]] = d;
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
