#include <iostream>
#include <unordered_map>
#include <vector>

#include "fmt/format.h"



void test_fmt() {
    std::string f1 = "{{\"status\": {}}}"; // ok
    std::string f2 = "{\"status\": {}}";   // not ok
//    std::string f3 = "{\"status\": 1}";    // not ok

    std::cout << fmt::format(f1, 1) << std::endl;
//    std::cout << fmt::format(f2, 2) << std::endl; // error: invalid format string
//    std::cout << fmt::format(f3) << std::endl;

    std::string f4 = "{{\n\"status\": {}\n}}";
    std::cout << "hello" << std::endl;
    std::cout << fmt::format("{} {}", "ok", "", "no") << std::endl;
    std::unordered_map<int, int> m{};
    m.emplace(1, 1);
    m.emplace(2, 1);
    m.emplace(3, 1);
    std::string res;
    for (auto &it: m) {
        if (res.length() > 0) res.append(",");
        res.append(std::to_string(it.first));
    }
    std::cout << res << std::endl;
    std::cout << fmt::format("{a}, {c}", fmt::arg("a", 1), fmt::arg("b", 2), fmt::arg("c", 3)) << std::endl;
}

void test_vector() {
    std::vector<int> datas{1,2,3,4,5};
    std::cout << datas.size() << " " << datas.capacity()<< std::endl;
    datas.reserve(10);
    std::cout << datas.size() << " " << datas.capacity()<< std::endl;
}

int main() {
    test_vector();
    return 0;
}
