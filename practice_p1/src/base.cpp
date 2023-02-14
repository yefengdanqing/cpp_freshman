#include "base.h"

#include <iostream>

namespace practice_p1 {
void Base::get_name() {
    std::cout << "base" << std::endl;
}
float Base::add_sum() {
    float result = m_sum_a + m_sum_b;
    std::cout << result << std::endl;
    return result;
}

int Base::get_index() {
    std::cout << index << std::endl;
    return index;
}
void Base::print() {
    std::cout << " xxxxxx" << std::endl;
}
}