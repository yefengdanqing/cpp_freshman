#pragma once

#include "system_a.h"
#include "system_b.h"

#include <iostream>
#include <memory>

class Facade {
public:
    Facade() {
        std::cout << "Facade()" << std::endl;
        system_a = std::make_shared<SystemA>();
        system_a = std::make_shared<SystemB>();
    }

    ~Facade() {
        std::cout << "~Facade()" << std::endl;
    }

    void work() {
        std::cout << "Facade::work()" << std::endl;
        system_a->work();
        system_a->work();
    }
private:
    std::shared_ptr<SystemA> system_a;
    std::shared_ptr<SystemB> system_b;
};