#include "library.h"

#include <iostream>

void Library::run() {
    clock();
    do_bussiness();
    get_off_work();
}
void Library::do_bussiness() {
    std::cout << "xxx" << std::endl;
}

void Library::clock() {
    std::cout << "clock time is" << std::endl;
}

void Library::get_off_work() {
    std::cout << "get_off_work time is" << std::endl;
}