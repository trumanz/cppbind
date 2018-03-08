#ifndef _CPPBIND__Json_file__
#define _CPPBIND__Json_file__

#include <fstream>     
#include <assert.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/json.h>

namespace  cppbind {


class JsonFile
{
public:
    std::string file_path;
    Json::Value jv;
public:
    JsonFile(){
    }
    Json::Value toJsonValue4Bind(){ 
        this->encodeFile(jv,this->file_path);
        return Json::Value(file_path);
    } 
    void fromJsonValue4Bind(const Json::Value& jv){
         assert(jv.isString());
         this->file_path = jv.asString();
         this->decodeFile(this->file_path);
   }
private:
    void encodeFile(const Json::Value& _jv, const std::string& _file_path){  
        std::fstream fs(_file_path, std::fstream::out);
        Json::StyledStreamWriter writer;
        writer.write(fs, _jv);
    } 

   void decodeFile(const std::string& file_path){
       std::fstream fs(file_path, std::fstream::in);
       Json::Value root;
       Json::Reader reader;
       bool parsingSuccessful = reader.parse(fs, root);
       if(!parsingSuccessful) {
		   std::string err_msg = reader.getFormattedErrorMessages();
         printf("Failed to parse, %s, %s\n", file_path.c_str(),err_msg.c_str());
         throw  CppBindException(file_path+ ":" + err_msg);
       }
       this->jv = root;        
   }
};

}

#endif
