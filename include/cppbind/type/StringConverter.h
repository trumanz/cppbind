#ifndef __CPPBIND__STRING_CONVERTER_
#define __CPPBIND__STRING_CONVERTER_
#include <typeinfo>   // operator typeid
#include <boost/any.hpp>
#include <sstream>
#include <map>
#include <assert.h>
#include <boost/shared_ptr.hpp>
namespace cppbind{

class StringConverter{
public:
    virtual std::string toString(boost::any any) = 0;
    virtual boost::any  fromString(const std::string& str) = 0;
    virtual ~StringConverter(){}
};


template<typename T, typename ConvertT>
class StringConverterT  : public StringConverter{
private:
    ConvertT x;
public:
     std::string toString(boost::any any){
         T e = boost::any_cast<T>(any);
         return x.toString(e);
    }
     boost::any  fromString(const std::string& str){
        return x.fromString(str);
    }
};


class StringConverterManager {
private:
    std::map<std::string, boost::shared_ptr<StringConverter> > converter_map;
public:
    template<typename T, typename ConverterT>
    void addStringConverter() {
        StringConverter* converter = new StringConverterT<T, ConverterT>();
        converter_map[typeid(T).name()] = boost::shared_ptr<StringConverter>(converter);
    }
    template<typename T>
    boost::shared_ptr<StringConverter> getStringConverter(){
        std::string ti = typeid(T).name();
        if(converter_map.find(ti) != converter_map.end()) {
            return converter_map[ti];
        }
        return boost::shared_ptr<StringConverter>();
    }
    template<typename T>
    std::string toString(T& e){
        boost::shared_ptr<StringConverter> converter = this->getStringConverter<T>();
        if(converter.get() == NULL) {
            std::stringstream ss;
            assert("BUG,  please register converter for T" == NULL);
        }
        return converter->toString(e);
    }

    template<typename T>
    T fromString(const std::string& str) {
        boost::shared_ptr<StringConverter> converter = this->getStringConverter<T>();
        if(converter.get() == NULL) {
            std::stringstream ss;
            ss << "BUG,  please register converter for " << typeid(T).name();
            assert(ss.str().c_str() == NULL);
        }
        return boost::any_cast<T>(converter->fromString(str));
    };
};


}


#endif

