#include <future>
#include <optional>
#include <vector>
#include <thread>
#include <chrono>

#include "base.h"



struct Res{
    int type;
    std::string id;
    std::string name;

};

int print_future(std::future<int>& fut);
int process_future_promise();

std::optional<Res> func_optional(std::vector<int>& result);

void asan_f();

void sleep_func();
void print_func();
int print_func2(std::string& name, int a, int b);
void print_thread_local_func(std::string& name);