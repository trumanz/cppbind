//
// Created by zck on 2022/5/2.
//

#include "cppbind/JsonDecoder.hpp"
#include "obj/Customer.h"
#include "obj/Order.h"
#include "gtest/gtest.h"



TEST(foregin_key_test, decode){
    Json::Value customer_list_json;
    customer_list_json.append(CustomerX::genJsonValueForTest(1001, "Zhang San", "Shanghai"));
    customer_list_json.append(CustomerX::genJsonValueForTest(1002, "Li Si", "Beijing"));

    Json::Value order_json;
    order_json["id"] = 9001;  order_json["customer"]=1001;

    cppbind::JsonDecoder jd;
    auto customers = jd.decode< std::vector< CustomerX* >  >(customer_list_json);
    ASSERT_EQ(customers->size(), 2);

    std::map<std::string, CustomerX*> customer_map;
    for(auto c : *customers) {
        customer_map[c->id] = c;
    }

    jd.regTable(&customer_map);
    auto order = jd.decode<  Order >(order_json);

    ASSERT_EQ(order->customer->name, "Zhang San");
    //free memory
    for(auto c :  *customers) {
        delete c;
    }
    delete customers;
    delete order;
};

