#pragma once
#include "product.h"


#include <iostream>


class ConcreteProductB : public Product {
private:
    /* data */
public:
    ConcreteProductB(/* args */) {}
    virtual ~ConcreteProductB() {}
    virtual void use() override { std::cout << "ConcreteProductB" << std::endl; }
};
