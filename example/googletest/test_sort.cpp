#include <thread>

#include "gtest/gtest.h"
#include "myfuture.h"
#include "test_class.h"





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
