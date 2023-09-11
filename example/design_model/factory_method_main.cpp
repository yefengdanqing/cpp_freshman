#include "factory_method/concrete_factory_a.h"
#include "factory_method/concrete_factory_b.h"



#include <iostream>
int main() {
    std::cout << "Hello, World!" << std::endl;
    auto factory_a = std::make_unique<ConcreteFactoryA>();
    factory_a->CreateProduct()->use();

    auto factory_b = std::make_unique<ConcreteFactoryB>();
    factory_b->CreateProduct()->use();
    return 0;
}