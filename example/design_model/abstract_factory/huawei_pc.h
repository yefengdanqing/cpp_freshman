#pragma once


#include "abstract_pc_product.h"

class HuaweiPc : public AbstractPcProduct {
public:
    HuaweiPc() {}
    ~HuaweiPc() {}
    void work() override {
        std::cout << "Huawei Pc" << std::endl;
    }
    void down() override {
        std::cout << "Huawei Pc down" << std::endl;
    }
};