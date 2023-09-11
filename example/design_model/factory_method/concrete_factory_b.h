#pragma once

#include "factory.h"
#include "concrete_product_b.h"


#include <memory>
class ConcreteFactoryB : public Factory {
public:
    ConcreteFactoryB() = default;
    virtual ~ConcreteFactoryB() = default;
    virtual std::shared_ptr<Product> CreateProduct() override {
        return std::make_shared<ConcreteProductB>();
    }
};

