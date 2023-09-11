#include <thread>
#include <functional>
#include <iostream>

#include "myfuture.h"

void print_future(std::future<int> *fut) {
    std::cout << "value ..." << std::endl;
    try
    {
        auto val = fut->get();
        std::cout << "id:" << std::this_thread::get_id() << ", value:" << val << std::endl;
    }
    catch(const std::future_error& e)
    {
        std::cerr << e.code() << " " << e.what() << '\n';
    }
    
}
int process_future_promise() {
    // std::promise<int> pro;
    // std::future<int> fut = pro.get_future();
    // std::thread t(print_future, &fut);
    // std::cout << "set promise before" << std::endl;
    // pro.set_value(100);
    // std::cout << "set promise after" << std::endl;

    // t.join();
    return 0;
}
std::optional<Res> func_optional(std::vector<int>& result) {
    if (result.size() == 0) {
        Res res;
        res.type = 1;
        res.id = "id";
        res.name = "name";
        std::cout << "id: " << res.id << std::endl;
        return std::move(res);
    } else {
        return std::nullopt;
    }
}

void asan_f()
{
    int a[2] = {1, 0};
    int b = a[1];
    // int b = a[2];
    std::cout << b << std::endl;
}

void sleep_func() {
    std::cout << "before sleep, print:" << 42 << std::endl;
    std::chrono::milliseconds duration(100);
    std::this_thread::sleep_for(duration);
    std::cout << "sleep end" << std::endl;
}

void print_func() {
    std::cout << "after sleep, print:" << 42 << ", id:" << std::this_thread::get_id() << std::endl;
}
int print_func2(std::string& name, int a, int b) {
    std::cout << "thread id :" << std::this_thread::get_id() << ", name:" << name << std::endl;
    return a+b;
}

void print_thread_local_func(std::string& name) {
    std::cout << "thread id :" << std::this_thread::get_id() << std::endl;
    Base a1;
    a1.increase(name);
    a1.increase(name);
    Base a2;
    a2.increase(name);
    a2.increase(name);
}
