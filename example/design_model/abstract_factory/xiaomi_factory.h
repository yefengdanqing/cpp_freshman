#pragma once

#include "abstract_factory.h"
#include "xiaomi_phone.h"
#include "xiaomi_tv.h"
#include "xiaomi_pc.h"


class XiaomiFactory : public AbstractFactory {
public:
    XiaomiFactory() {}
    ~XiaomiFactory() {}
    //不一定都有
    std::shared_ptr<AbstractPhoneProduct> create_phone() override {
        return std::make_shared<XiaomiPhone>();
    }
    std::shared_ptr<AbstractTvProduct> create_tv() override {
        return std::make_shared<XiaomiTv>();
    }
    std::shared_ptr<AbstractPcProduct> create_pc() override {
        return std::make_shared<XiaomiPc>();
    }

};