#include <functional>
#include <type_traits>


#include "gtest/gtest.h"
#include "myfuture.h"


TEST(Function, FunctionPtr) {
    typedef int (*print_func_ptr)(std::string& name, int a, int b);
    print_func_ptr ptr = print_func2;
    std::string abc = "functional_ptr";
    ptr(abc, 42, 1);

}
TEST(Function, FunctionFunc) {
    std::function<int(std::string&,int,int)> func;
    func = print_func2;
    std::string abc = "functional_func";
    func(abc, 42, 1);

}
TEST(Function, FunctionFuncPtr) {
    using func_ptr = int (*)(std::string&, int, int);
    std::function<int(std::string&, int, int)> func;
    ASSERT_EQ(1,1);
    bool is_int = std::is_same<int, int>::value;
    EXPECT_TRUE(is_int);
    bool type_result = std::is_same<func_ptr, std::function<int(std::string&, int, int)>>::value;
    bool type_result2 = std::is_same<decltype(func), std::function<int(std::string&, int, int)>>::value;
    EXPECT_TRUE(type_result2);
    // EXPECT_TRUE(type_result);
    EXPECT_FALSE(type_result);
    func_ptr ptr = print_func2;
    //可以把函数指针赋值给
    func = ptr;
    std::string abc = "functional_func_ptr";
    ptr(abc, 42, 1);
    func(abc, 42, 1);

}