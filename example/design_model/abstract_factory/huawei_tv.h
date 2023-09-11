#pragma once

#include "abstract_tv_product.h"

class HuaweiTv : public AbstractTvProduct {
public:
    HuaweiTv() {}
    ~HuaweiTv() {}
    void play() override {
        std::cout << "Huawei Tv play" << std::endl;
    }
    void open() override {
        std::cout << "Huawei Tv open" << std::endl;
    }
};