#include <thread>

#include "gtest/gtest.h"
#include "myfuture.h"
#include "test_class.h"

//./google_test --gtest-filter=abc.123
//abc是测试套件名字，123是测试用例名字
//main函数必须有参数，否则没办法传入
int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    return result;
}
