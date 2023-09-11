#pragma once
#include "factory.h"
#include "product.h"
#include "concrete_product_a.h"

#include <iostream>
#include <memory>

class ConcreteFactoryA : public Factory
{
public:
    ConcreteFactoryA() {}
    virtual ~ConcreteFactoryA() {}
    virtual std::shared_ptr<Product> CreateProduct() override {
        return std::make_shared<ConcreteProductA>();
    }

};