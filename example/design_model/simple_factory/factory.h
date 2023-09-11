#ifndef SIMPLE_FACTORY_FACTORY_H__
#define SIMPLE_FACTORY_FACTORY_H__

#include "product.h"
#include "product_a.h"
#include "product_b.h"

#include <string>
#include <memory>
#include <iostream>

class Factory
{
public:
    static std::shared_ptr<Product> create(const std::string& name);
    virtual ~Factory() = default;
};



#endif