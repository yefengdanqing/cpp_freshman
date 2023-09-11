#pragma once

#include "abstract_phone_product.h"

class XiaomiPhone : public AbstractPhoneProduct {
public:
    XiaomiPhone() {}
    ~XiaomiPhone() {}
    void call() override {
        std::cout << "Xiaomi Phone call" << std::endl;
    }
    void bell() override {
        std::cout << "Xiaomi Phone bell" << std::endl;
    }
};