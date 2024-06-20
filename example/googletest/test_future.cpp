#include <thread>

#include "gtest/gtest.h"
#include "myfuture.h"
#include "test_class.h"

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