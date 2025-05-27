#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include <new>
#include <version>


// 存在伪共享的结构体
struct FalseSharingData {
    std::atomic<int> counter1;
    std::atomic<int> counter2; // 与counter1很可能在同一个cache line
};

void false_sharing_example() {
    FalseSharingData data{};
    data.counter1 = 0;
    data.counter2 = 0;

    auto work = [](std::atomic<int>& counter, int iterations) {
        for (int i = 0; i < iterations; ++i) {
            ++counter;
        }
    };

    const int iterations = 100'000'000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::thread t1(work, std::ref(data.counter1), iterations);
    std::thread t2(work, std::ref(data.counter2), iterations);
    
    t1.join();
    t2.join();
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    
    std::cout << "False sharing time: " << duration.count() << " seconds\n";
    std::cout << "Counter1: " << data.counter1 << ", Counter2: " << data.counter2 << "\n";
}

// 修复伪共享的结构体
struct AlignedData {
    alignas(64) std::atomic<int> counter1; // 64字节对齐，确保单独cache line
    alignas(64) std::atomic<int> counter2; // 另一个cache line
};

void no_false_sharing_example() {
    AlignedData data{};
    data.counter1 = 0;
    data.counter2 = 0;

    auto work = [](std::atomic<int>& counter, int iterations) {
        for (int i = 0; i < iterations; ++i) {
            ++counter;
        }
    };

    const int iterations = 100'000'000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::thread t1(work, std::ref(data.counter1), iterations);
    std::thread t2(work, std::ref(data.counter2), iterations);
    
    t1.join();
    t2.join();
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    
    std::cout << "No false sharing time: " << duration.count() << " seconds\n";
    std::cout << "Counter1: " << data.counter1 << ", Counter2: " << data.counter2 << "\n";
}

// 打印缓存行大小
void print_cache_line_size() {
    // std::cout << "Cache line size: " << std::hardware_destructive_interference_size 
    //           << " bytes\n";
}

int main() {
    print_cache_line_size();
    
    std::cout << "=== False Sharing Example ===\n";
    false_sharing_example();
    
    std::cout << "\n=== No False Sharing Example ===\n";
    no_false_sharing_example();
    
    return 0;
}

