#pragma once
#include "product.h"

#include <iostream>

class ConcreteProductA : public Product {
public:
    ConcreteProductA() {}
    virtual ~ConcreteProductA() {}
    virtual void use() {
        std::cout << "ConcreteProductA" << std::endl;
    }

};