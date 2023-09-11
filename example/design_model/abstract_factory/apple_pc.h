#pragma once
#include "abstract_pc_product.h"

class ApplePc : public AbstractPcProduct{
public:
    ApplePc() {}
    ~ApplePc() {}
    void work() override {
        std::cout << "Apple Pc" << std::endl;
    }
    void down() override {
        std::cout << "Apple Pc down" << std::endl;
    }

};