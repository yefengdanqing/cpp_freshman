#pragma once

#include "abstract_phone_product.h"

class ApplePhone : public AbstractPhoneProduct {
public:
    ApplePhone() {}
    ~ApplePhone() {}
    void call() override {
        std::cout << "Apple Phone call" << std::endl;
    }
    void bell() override {
        std::cout << "Apple Phone bell" << std::endl;
    }
};