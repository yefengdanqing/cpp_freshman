#include "simple_factory/factory.h"
#include "simple_factory/product_a.h"
#include "simple_factory/product_b.h"

#include <iostream>
#include <memory>

int main() {
    std::shared_ptr<Product> product_a = Factory::create("ProductA");
    product_a->who();

    std::shared_ptr<Product> product_b = Factory::create("ProductB");
    product_b->who();

    return 0;
}