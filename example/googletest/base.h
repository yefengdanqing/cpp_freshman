#pragma once
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <algorithm>


class Base {
public:
    Base() {
        std::cout << "thread id :" << std::this_thread::get_id() << "," << __FUNCTION__ << "-(" << this << ")" << std::endl;
    }
    Base(const std::string& info) {
        std::cout << info << std::endl;
    }
    ~Base() {}
    void increase(std::string& name) {
        thread_local int count = 1;
        count++;
        std::cout << name << ", obj:" << count << std::endl;
    }
};

struct TestData {
    int a;
    int b;
    std::string name;
    bool operator==(const TestData& other) const {
        return a == other.a && b == other.b && name == other.name;
    }
    bool operator<(const TestData& other) const {
        return a < other.a && b < other.b;
    }
    bool operator>(const TestData& other) const {
        return a > other.a && b > other.b;
    }
};
