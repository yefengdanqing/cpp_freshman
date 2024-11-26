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

TEST(SkillTrait1, SkillTraitVector1) {
	std::cout << "SkillTrait1.SkillTraitVector1" << std::endl;
    std::vector<TestData> datas;
	datas.push_back(TestData{1, 2, "abc"});
	datas.push_back(TestData{2, 3, "abc"});
	datas.push_back(TestData{5, 6, "abc"});
	datas.push_back(TestData{4, 5, "abc"});
	datas.push_back(TestData{5, 6, "abc"});
	datas.push_back(TestData{3, 4, "abc"});
	std::cout << datas.size() << "abcabc" << std::endl;
    datas.~vector();
	std::cout << datas.size() << "abcabc" << std::endl;

    // EXPECT_EQ(6, datas.size());//已经析构了，取到的数是未定义的
    new (&datas)::std::vector<TestData>{1};
    datas.push_back(TestData{1, 2, "abc"});
    EXPECT_EQ(2, datas.size());
    std::cout << datas.size() << "abcabc" << std::endl;
}