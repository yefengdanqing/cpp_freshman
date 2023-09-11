#include <functional>
#include "gtest/gtest.h"


int Factorial(int n);

// 仿函数
class CompareObject {
public:
    int operator()() {
      return x;
    }
    int operator() (int a) {
        return x + a;
    }
    int x = 0;

};
class Base {
public:
    int add(int a) {
        return a * a;
    }
};



