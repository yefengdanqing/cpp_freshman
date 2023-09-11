#pragma once

#include "abstract_factory.h"
#include "apple_phone.h"
#include "apple_tv.h"
#include "apple_pc.h"

class AppleFactory : public AbstractFactory {
public:
    AppleFactory() {}
    ~AppleFactory() {}
    std::shared_ptr<AbstractPhoneProduct> create_phone() override {
        return std::make_shared<ApplePhone>();
    }
    std::shared_ptr<AbstractTvProduct> create_tv() override {
        return std::make_shared<AppleTv>();
    }
    std::shared_ptr<AbstractPcProduct> create_pc() override {
        return std::make_shared<ApplePc>();
    }
};