#pragma once

#include "abstract_tv_product.h"

class XiaomiTv : public AbstractTvProduct {
public:
    XiaomiTv() {}
    ~XiaomiTv() {}
    void play() override {
        std::cout << "Xiaomi Tv play" << std::endl;
    }
    void open() override {
        std::cout << "Xiaomi Tv open" << std::endl;
    }
};