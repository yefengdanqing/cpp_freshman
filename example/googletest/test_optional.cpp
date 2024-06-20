#include <thread>

#include "gtest/gtest.h"
#include "myfuture.h"
#include "test_class.h"

TEST(asan, asan_test) {
    asan_f();
}

TEST(optional, optional_func) {
    std::vector<int> datas;
    const auto& res = func_optional(datas);
    // auto v = res.value();
    if (res.has_value()) {
        auto v = res.value();
        EXPECT_EQ("name", v.name);
    }
}


TEST(optional, optional_opt) {
	std::string str = "unit_test";
    UnitTestClass<std::string> unit(str);
	// UnitTestClass<std::string> unit1 = std::move(unit);

	std::cout << "*****************************************\n";
	std::cout << "Use Optional:\n";
	std::cout << "copy:\n";
	OptFunc<std::string>(unit);
	std::cout << "-------------\n";
	std::cout << "move:\n";
	OptFunc(std::optional<UnitTestClass<std::string>>("unit_test1"));
	std::cout << "*****************************************\n";
	std::cout << "Not use Optional:\n";
	std::cout << "copy:\n";
	DefaultFunc(unit);
	std::cout << "-----------------\n";
	std::cout << "move:\n";
	DefaultFunc(std::move(UnitTestClass<std::string>("unit_test2")));
	std::cout << "*****************************************\n";
}