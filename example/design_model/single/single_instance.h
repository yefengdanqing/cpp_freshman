#pragma once

#include <iostream>

class SingleInstance {
private:
    SingleInstance() {
        std::cout << "SingleInstance()" << std::endl;
    }

    ~SingleInstance() {
        std::cout << "~SingleInstance()" << std::endl;
    }

    
public:

};