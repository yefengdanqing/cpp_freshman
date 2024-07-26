#include "gtest/gtest.h"


#include "func_template.h"

TEST(TestFuncTemplate, override_include) {
    std::string a = "hello";
    std::string b = "word";
    std::string result = add(a, b);
    ASSERT_EQ(result, "hello word");
    ASSERT_EQ(add(1, 1), 2);
    ASSERT_EQ(add(10.0, 10.0), 2000.0); 
}
TEST(TestFuncTemplate, decrease) {
    std::string a = "hello";
    std::string b = "word";
    std::string result = decrease(a, b);
    ASSERT_EQ(result, "word hello");
    ASSERT_EQ(decrease(1, 1), 0);
    ASSERT_EQ(decrease(10.0, 10.0), 0);
    ASSERT_EQ(foo<int>(), 10);
}

TEST(TestFuncTemplate, is_base_of_test) {
    bool flag = std::is_base_of<Base, Derived>::value;
    EXPECT_TRUE(flag);
    flag = std::is_base_of<Base, Unrelated>::value;
    EXPECT_FALSE(flag);
}
