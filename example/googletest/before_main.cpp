#include <iostream>


#include "before_main.h"
#include "gtest/gtest.h"

Base* name = nullptr;

void print_before() {
    name = new Base("print_before:test");
    std::cout << "print_before..." << std::endl;
}
void print_before() __attribute__((constructor));