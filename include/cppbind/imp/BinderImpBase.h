//
// Created by zck on 2022/5/2.
//

#ifndef CPPBIND_BINDERIMPBASE_H
#define CPPBIND_BINDERIMPBASE_H

class BinderImpBase {
public:
    StringConverterManager str_convert_mgmt;
    std::map<std::string, boost::shared_ptr<ForeignTableInterface> > foreign_tables;
    ClassRegisterBase* class_reg;
public:
    BinderImpBase() {
        this->class_reg = NULL;
        str_convert_mgmt.addStringConverter<boost::posix_time::ptime,BoostPTimeConverter>();
        str_convert_mgmt.addStringConverter<boost::posix_time::time_duration,BoostTimeDurationConverter>();
        str_convert_mgmt.addStringConverter<boost::gregorian::date,BoostGDateConverter>();
    }
    virtual ~BinderImpBase() {}
private:
    template<typename ObjT>
    void regForeignTable(ForeignTableInterface* tabe){
        this->foreign_tables[ typeid(ObjT).name()] =  boost::shared_ptr<ForeignTableInterface>(tabe);
    }
public:
    template<typename ObjT>
    void regForeignTable(const std::map<std::string, ObjT*> *table) {
        boost::shared_ptr<SimpleMapTable<ObjT> > t(new SimpleMapTable<ObjT>(table));
        this->regForeignTable<ObjT, SimpleMapTable<ObjT> >(t);
    }
    template<typename ObjT, typename TableT>
    void regForeignTable(boost::shared_ptr<TableT> ft){
        this->regForeignTable<ObjT>(new ForeginTableT<ObjT,TableT>(ft));
    }

public:
    void regClassRegister(ClassRegisterBase* _class_reg){
        assert(this->class_reg == NULL);
        this->class_reg = _class_reg;
    }
    template<typename T>
    T* getForeignObj(std::string key) {
        const std::string type_name = typeid(T).name();
        std::map<std::string, boost::shared_ptr<ForeignTableInterface> >::iterator it = this->foreign_tables.find(type_name);
        if(it == this->foreign_tables.end()) {
            printf("ERROR, Please register table for %s\n", type_name.c_str());
            assert(false);
        }
        boost::any any_obj = it->second->getAnyObj(key);
        T* obj = boost::any_cast<T*>(any_obj);
        if (obj == NULL) {
            printf("can not find foreign obj %s", key.c_str());
            throw ForeginKeyMissingException(key);
        }
        return obj;
    }

};

#endif //CPPBIND_TEST_BINDERIMPBASE_H
