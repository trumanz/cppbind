//
// Created by zck on 2022/5/2.
//

#pragma once

#include "Customer.h"


class Order {
public:
    int id;
    CustomerX* customer;
    void setBind(Binder *binder, bool load){
        binder->bind("id", id);
        auto getKey = [=]() { return this->customer->id;};
        binder->bindForeginKey("customer", customer, getKey);
    }
    //std::map<Product*, int>  product_count;
};



