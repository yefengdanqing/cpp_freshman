#pragma once

#include "abstract_phone_product.h"
class HuaweiPhone : public AbstractPhoneProduct {
public:
    HuaweiPhone() {}
    ~HuaweiPhone() {}
    void call() override {
        std::cout << "Huawei Phone call" << std::endl;
    }
    void bell() override {
        std::cout << "Huawei Phone bell" << std::endl;
    }
};