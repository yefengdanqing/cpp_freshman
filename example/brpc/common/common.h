#pragma once
#include <iostream>

#define CHECT_RET_EXIT(ret, n)\
    if (ret != 0) { \
        std::cout << "error: " << n << std::endl; \
        exit(1); \
    }