#ifndef _CPPBIND__Json_file__
#define _CPPBIND__Json_file__

#include <iostream>
#include <fstream>     
#include <assert.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/json.h>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include "cppbind/cppbind_exception.h"
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
        std::fstream fs(_file_path.c_str(),  std::fstream::out);
        Json::StyledStreamWriter writer;
        writer.write(fs, _jv);
    } 

   void decodeFile(const std::string& file_path){
       std::fstream fs(file_path.c_str(),  std::fstream::in);
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
// a json list; ["r:

class MultiJsonFile {
public:
    std::vector<JsonFile> jfs;
public:
    Json::Value toJsonValue4Bind(){
        Json::Value jv(Json::arrayValue);
        for(size_t i = 0; i < jfs.size(); i++) {
            JsonFile& jf = jfs[i];
            jv.append(jf.toJsonValue4Bind());
        }
        return jv;
    } 
    void fromJsonValue4Bind(const Json::Value& jv){
        assert(jv.isArray());
        std::set<boost::filesystem::path> files_set;
        std::vector<boost::filesystem::path> files_vec;
        for(Json::Value::ArrayIndex i = 0; i  < jv.size(); i++) {
            std::vector<boost::filesystem::path> x = this->getFiles(jv[i]);
            for(size_t i = 0; i < x.size(); i++) {
                boost::filesystem::path& path = x[i];
                if(files_set.find(path) == files_set.end()) {
                    files_vec.push_back(path);
                    files_set.insert(path);
                }
            }
        }
        this->laodJsonFiles(files_vec);
   }
private:
    void laodJsonFiles(std::vector<boost::filesystem::path> files){
        for(size_t j = 0; j < files.size(); j++) {
           boost::filesystem::path f = files[j];
           JsonFile jf;
           std::string str = f.string();
           std::cout << "fstr: " << str << "\n";
           jf.fromJsonValue4Bind(Json::Value(f.string()));
           std::cout << str << "\n";
           jfs.push_back(jf);
        }
   }
    std::vector<boost::filesystem::path> getFiles(const Json::Value& jv){
        assert(jv.isString());
        std::string file_pattern = jv.asString();
        printf("file_pattern=%s\n", file_pattern.c_str());
        std::vector<boost::filesystem::path> files = this->findMatchFiles(file_pattern);
        return files;
    }
   std::vector<boost::filesystem::path> 
        findMatchFiles(std::string pattern,
                       boost::filesystem::path _base_path = boost::filesystem::current_path()){

       std::vector<boost::filesystem::path> match_paths;
       boost::regex filter(pattern);
       boost::filesystem::path base_path = boost::filesystem::canonical(_base_path);
       const size_t  base_path_str_len = base_path.string().length();
       boost::filesystem::recursive_directory_iterator end;
       for (boost::filesystem::recursive_directory_iterator it(base_path); it != end; ++it)
       {
          const boost::filesystem::path p = (*it);
          if(boost::filesystem::is_regular_file(p) || boost::filesystem::is_symlink(p)) {
              boost::smatch what;
              std::string relative_path = p.string().substr(base_path_str_len + 1);
              if( !boost::regex_match( relative_path, what, filter ) ) continue;
              match_paths.push_back(p);
          }
       }
       return match_paths;
   }
};


}

#endif
