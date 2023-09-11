#pragma once

#include "abstract_pc_product.h"
class XiaomiPc : public AbstractPcProduct {
public:
    XiaomiPc() {}
    ~XiaomiPc() {}
    void work() override {
        std::cout << "Xiaomi Pc" << std::endl;
    }
    void down() override {
        std::cout << "Xiaomi Pc down" << std::endl;
    }
};