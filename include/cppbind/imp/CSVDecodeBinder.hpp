﻿#ifndef __CSV_DECODE_BINDER_H__
#define __CSV_DECODE_BINDER_H__
#if 0
namespace  cppbind {


class CSVDecodeBinder : public EnconderDecoderBase {
public:
    
    CSVDecodeBinder(const std::vector<std::string>& row){
        this->csv_row = row;
        this->used = 0;
    }

    template<typename T>
    void decode(T* e){
        this->decode(this->csv_row.begin(), this->csv_row.end(), e);
    }

    template<typename T>
    void bind(const std::string& name, T& v){
        if(this->csv_row.begin() + used ==  this->csv_row.end()) {
            throw CppBindException(std::string(".") + name, " not enought cell could be used for csv decode");
        }
        std::string sv = this->csv_row[used];
        used += decode(this->csv_row.begin() + used, this->csv_row.end(), &v);
    }

    template<typename T>
    void bind(const std::string& name, boost::shared_ptr<T>& v){
        if(used+1 ==  this->csv_row.size()) { return; }
        T e;
        bind(name,e);
        v = boost::shared_ptr<T>(new T(e));
    }

    

private: //for std container type
    template<typename T>
    size_t decode(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, std::vector<T>* e){
         throw CppBindException("csv not support  vector");
         return 0;
    }
    template<typename T>
    size_t decode(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, std::list<T>* e){
        throw CppBindException("csv not support  list");
        return 0;
    }
    template<typename T>
    size_t decode(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, std::set<T>* e){
        throw CppBindException("csv not support  set");
        return 0;
    }
    template<typename T>
    size_t decode(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, std::map<std::string, T>* e){
             throw CppBindException("csv not support map");
             return 0;
    }
private: // for class type


    class MemberFunctionCaller{
    public:
        template<typename T>
        void call(T*e, Binder* binder){
            e->setBind(binder);
        }
    };

    class RunTimeBinderCaller{
    public:
        template<typename T>
        void call(T*e, Binder* binder){
            JsonDecoderImp* json_decode_binder = dynamic_cast<JsonDecoderImp*>(binder->binder_imp);
            const Json::Value& jv = json_decode_binder->binder->json_to_decode;
            if(!jv.isString()) {
                assert("bug" == nullptr);
            }
            std::string str = jv.asString();
            e[0] = binder->str_convert_mgmt.fromString<T>(str);
         }

    };

    template<typename T>
    size_t decode(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, T* e){
           std::vector<std::string> x;
           x.insert(x.begin(),  begin, end);
           Binder binder(boost::shared_ptr<EnconderDecoderBase>(new CSVDecodeBinder(x)));
           e->setBind(&binder);
           CSVDecodeBinder*  csv_decode_binder  = dynamic_cast<CSVDecodeBinder*>(binder.binder_imp);
          return  csv_decode_binder->used;
    } 
private:  //for basic type
    size_t decode(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, bool*e){
        return decode_basic_type(*begin, e);
    }
    size_t decode(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, int32_t *e){
         return decode_basic_type(*begin, e);
    }
    size_t decode(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, int64_t *e){
          return decode_basic_type(*begin, e);
    }
    size_t decode(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, float *e){
        return decode_basic_type(*begin, e);
    }
    size_t decode(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end,  double *e){
        return decode_basic_type(*begin, e);
    }
    size_t decode(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, std::string *e){
        return decode_basic_type(*begin, e);
    }
    template<typename T>
    size_t decode_basic_type(const std::string &content, T*e){
        std::stringstream ss;
        ss << content;
        ss >>  *e;
        return 1;
    }
 public:
    std::vector<std::string> csv_row;
    size_t used;
};

}
#endif
#endif
