#include "factory.h"
#include "product_a.h"
#include "product_b.h"

#include <iostream>


std::shared_ptr<Product> Factory::create(const std::string& name) {
    if (name == "ProductA") {
        return std::make_shared<ProductA>();
    } else if (name == "ProductB") {
        return std::make_shared<ProductB>();
    } else {
        std::cout << "No such product" << std::endl;
        return nullptr;
        // return std::make_shared<Product>();
    }
}

