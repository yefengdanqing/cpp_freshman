#include <iostream>

#include <babylon/concurrent/deposit_box.h>

using ::babylon::DepositBox;

int main() {
    auto& box = DepositBox<int>::instance();
    auto id = box.emplace(1111);
    {
        auto accessor = box.take_released(id);
        std::cout << "result:" << *accessor << std::endl;
    }
    return 0;
}