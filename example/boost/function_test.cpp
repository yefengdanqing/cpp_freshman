#include "function_test.h"

std::function<int(int)>  Callback;
int Factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; i++) {
      result *= i;
    }

    return result;
}

auto func_lambda = [](int a)->int { return a+10; };


TEST(FUNCTION_TEST, POSITIVE) {
    Callback = Factorial;
    ASSERT_EQ(1, Callback(-1));
    Callback = func_lambda;
    ASSERT_EQ(10, Callback(0));
    CompareObject obj;
    Callback = obj;
    std::cout << obj(10) << std::endl;
    ASSERT_EQ(10, Callback(10));

    Base a1;
    Callback = std::bind(&Base::add, a1, std::placeholders::_1);
    ASSERT_EQ(4, Callback(2));

}
