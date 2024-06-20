#include <functional>

#include "gtest/gtest.h"
#include "myfuture.h"


TEST(Function, FunctionPtr) {
    typedef int (*print_func_ptr)(std::string& name, int a, int b);
    print_func_ptr ptr = print_func2;
    std::string abc = "functional_ptr";
    ptr(abc, 42, 1);


}
TEST(Function, FunctionFunction) {
    std::function<int(std::string&,int,int)> func;
    func = print_func2;
    std::string abc = "functional_func";
    func(abc, 42, 1);

}