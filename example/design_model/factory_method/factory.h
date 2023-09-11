#pragma once
#include "product.h"

#include <memory>

class Factory
{
public:
    Factory() {}
    ~Factory() {}
    virtual std::shared_ptr<Product> CreateProduct() = 0;
};
