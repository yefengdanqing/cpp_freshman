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

TEST(async, async_func1) {
	auto f1 = std::async(std::launch::async, []{sleep_func();});
	auto f2 = std::async(std::launch::async, []{print_func();});

}

TEST(thread_local, thread_local_1) {
	std::string name("func1");
	std::thread t(&print_thread_local_func, std::ref(name));
	
	std::string name1 = "func2";
	std::thread t1(&print_thread_local_func, std::ref(name1));
	t1.join();
	t.join();

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
TEST(sort, point_struct) {
	std::vector<TestData> datas;
	datas.push_back(TestData{1, 2, "abc"});
	datas.push_back(TestData{2, 3, "abc"});
	datas.push_back(TestData{5, 6, "abc"});
	datas.push_back(TestData{4, 5, "abc"});
	datas.push_back(TestData{5, 6, "abc"});
	datas.push_back(TestData{3, 4, "abc"});

	std::vector<TestData*> datas1;
	for (auto& data : datas) {
		std::cout << data.a << "," << data.b << "," << data.name << std::endl;
		datas1.push_back(&data);
	}
	std::cout << "---------------------\n";

	std::sort(datas1.begin(), datas1.end(), [](TestData* a, TestData* b) {
		return *a < *b;
	});

	for (auto& data : datas1) {
		std::cout << data->a << "," << data->b << "," << data->name << std::endl;
	}
	std::cout << "**********************\n";

	for (auto& data : datas) {
		std::cout << data.a << "," << data.b << "," << data.name << std::endl;
	}
	
	
}

TEST(optimization, obj_copy) {
	auto tmp = get_copy_obj<int>(42);
	std::cout << tmp.data << std::endl;
}

TEST(future, future_first) {
    EXPECT_EQ(0, process_future_promise());
}
TEST(packaged_task, packaged_task_future) {
	std::packaged_task<int(std::string&, int, int)> task(print_func2);
	auto fut = task.get_future();
	std::string name = "abc";
	std::thread t(std::move(task), std::ref(name), 42, 42);
	std::cout << "value:" << fut.get() << std::endl;
	t.join();
	std::packaged_task<void()> task1;
	std::cout << "result status:" << task1.valid() << std::endl;
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
    return 0;
}
