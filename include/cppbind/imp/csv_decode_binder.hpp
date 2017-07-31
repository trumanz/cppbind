#ifndef __CSV_DECODE_BINDER_H__
#define __CSV_DECODE_BINDER_H__

namespace  cppbind {


class CSVDecodeBinder : public BinderImpBase {
public:
    CSVDecodeBinder(Json::Value json){
        //this->json = json;
     }
    void setJson(const Json::Value& jv){
     }
    CSVDecodeBinder(const std::vector<std::string>& row){
        this->csv_row = row;
        this->used = 0;
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
        if(used+1 ==  this->csv_row.size()) {
            return;
        }
        T  e;
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
    template<typename T>
    size_t decode(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, T* e){
           std::vector<std::string> x;
           x.insert(x.begin(),  begin, end);
           Binder binder(boost::shared_ptr<BinderImpBase>(new CSVDecodeBinder(x)));
           e->setBind(&binder);
           CSVDecodeBinder*  csv_decode_binder  = dynamic_cast<CSVDecodeBinder*>(binder.binder_imp.get());
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
