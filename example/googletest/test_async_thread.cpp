#include <thread>

#include "gtest/gtest.h"
#include "myfuture.h"
#include "test_class.h"

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