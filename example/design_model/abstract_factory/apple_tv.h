#pragma once

#include "abstract_tv_product.h"

class AppleTv : public AbstractTvProduct {
public:
    AppleTv() {}
    ~AppleTv() {}
    void play() override {
        std::cout << "Apple Tv play" << std::endl;
    }
    void open() override {
        std::cout << "Apple Tv open" << std::endl;
    }
};