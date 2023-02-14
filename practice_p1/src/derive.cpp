#include "derive.h"
#include <iostream>


namespace practice_p1 {
void Derive::get_name() {
    std::cout << "Derive" << std::endl;
}
float Derive::add_sum() {
    float result = m_sum_a + m_sum_b;
    std::cout << result << std::endl;
    return result;
}
int Derive::get_index() {
    std::cout << index << std::endl;
    return index;
}
void Derive::print1() {
    std::cout << " yyyyyy" << std::endl;
}
}