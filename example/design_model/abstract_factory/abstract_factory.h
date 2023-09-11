#pragma once
#include "abstract_phone_product.h"
#include "abstract_tv_product.h"
#include "abstract_pc_product.h"

#include <iostream>
#include <memory>


class AbstractFactory {
    virtual std::shared_ptr<AbstractPhoneProduct> create_phone() = 0;
    virtual std::shared_ptr<AbstractTvProduct> create_tv() = 0;
    virtual std::shared_ptr<AbstractPcProduct> create_pc() = 0;

};