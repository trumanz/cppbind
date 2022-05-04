//
// Created by zck on 2022/5/2.
//

#ifndef CPPBIND_TEST_CUSTOMER_H
#define CPPBIND_TEST_CUSTOMER_H

using namespace cppbind;

class CustomerX {
public:
    std::string id; //todo try other type
    std::string name;
    std::string address;
    void setBind(Binder *binder, bool load){
        binder->bind("id", id);
        binder->bind("name", name);
        binder->bind("address", address);
    }
    //std::string getKeyStr() {
    //    return id;
    //}

    static  Json::Value genJsonValueForTest(int id,
                                            const std::string& name,
                                            const std::string& address) {
        Json::Value jv;
        jv["id"] = std::to_string(id);
        jv["name"] =  name;
        jv["address"] = address;
        return jv;
    };
};


#endif //CPPBIND_TEST_CUSTOMER_H
