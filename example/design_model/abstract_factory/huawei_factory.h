#pragma once

#include "abstract_factory.h"

#include "huawei_phone.h"
#include "huawei_tv.h"
#include "huawei_pc.h"

class HuaweiFactory : public AbstractFactory {
public:
    HuaweiFactory() {}
    ~HuaweiFactory() {}
    std::shared_ptr<AbstractPhoneProduct> create_phone() override {
        return std::make_shared<HuaweiPhone>();
    }
    std::shared_ptr<AbstractTvProduct> create_tv() override {
        return std::make_shared<HuaweiTv>();
    }
    std::shared_ptr<AbstractPcProduct> create_pc() override {
        return std::make_shared<HuaweiPc>();
    }
};