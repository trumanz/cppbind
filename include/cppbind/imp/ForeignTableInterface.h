//
// Created by zck on 2022/5/2.
//

#ifndef CPPBIND_FOREIGNTABLEINTERFACE_H
#define CPPBIND_FOREIGNTABLEINTERFACE_H

template<typename T>
class SimpleMapTable{
    const std::map<std::string, T*>* table;
public:
    SimpleMapTable(const std::map<std::string, T*>* _table) { this->table = _table;}
    T* getObj(std::string key){
        typename std::map<std::string, T*>::const_iterator it = table->find(key);
        if(it == table->end()) {
            printf("ERROR, Can not found %s", key.c_str());
            throw cppbind::ForeginKeyMissingException(key);
        }
        T* x = it->second;
        return x;
    }
};

class ForeignTableInterface {
public:
    virtual ~ForeignTableInterface(){}
    virtual boost::any getAnyObj(std::string key) = 0;
};


template<typename ObjT, typename TableT>
class ForeginTableT : public ForeignTableInterface{
    boost::shared_ptr<TableT> table;
public:
    ForeginTableT(boost::shared_ptr<TableT> _table) {
        this->table = _table;
    }
    boost::any getAnyObj(std::string key){
        ObjT* obj =  table->getObj(key);
        boost::any x = obj;
        return x;
    }
};

#endif //CPPBIND_TEST_FOREIGNTABLEINTERFACE_H
