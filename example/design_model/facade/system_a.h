#pragma once

#include <iostream>

class SystemA {
public:
    SystemA() {
        std::cout << "SystemA()" << std::endl;
    }

    ~SystemA() {
        std::cout << "~SystemA()" << std::endl;
    }

    void work() {
        std::cout << "SystemA::work()" << std::endl;
    }
};