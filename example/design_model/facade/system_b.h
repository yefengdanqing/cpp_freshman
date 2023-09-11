#pragma once

#include <iostream>

class SystemB {
public:
    SystemB() {
        std::cout << "SystemB()" << std::endl;
    }

    ~SystemB() {
        std::cout << "~SystemB()" << std::endl;
    }

    void work() {
        std::cout << "SystemB::work()" << std::endl;
    }
};
