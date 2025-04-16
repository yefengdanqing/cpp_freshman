#include <string>
#include <iostream>
#include <babylon/concurrent/bounded_queue.h>
#include <bthread/bthread.h>
#include <thread>


using ::babylon::ConcurrentBoundedQueue;
ConcurrentBoundedQueue<std::string> queue;

void* put_data_by_thread(void* args) {
    ConcurrentBoundedQueue<std::string>* queue_ptr = (ConcurrentBoundedQueue<std::string>*)(args);
    std::string data;
    for (int i = 0; i < 2000; i++) {
        data = "data:" + std::to_string(i);
        std::cout << "input:" << data << std::endl;
        queue_ptr->push(data);
    }
    return nullptr;
}
void* get_data_by_thread(void* args) {
    ConcurrentBoundedQueue<std::string>* queue_ptr = (ConcurrentBoundedQueue<std::string>*)(args);
    std::string result;
    for (int i = 0; i < 2000; i++) {
        result.clear();
        //queue_ptr->pop(result);
        // std::cout << "output:" << result << std::endl;
    }
    return nullptr;
}
int main() {
    queue.reserve_and_clear(1000);
    std::cout << queue.capacity() << std::endl;
    bthread_t td1;
    bthread_t td2;
    int ret2 = bthread_start_background(&td2, NULL, get_data_by_thread, (void*)(&queue));
    int ret1 = bthread_start_background(&td1, NULL, put_data_by_thread, (void*)(&queue));
    queue.push("10086");
    std::string result;
    queue.pop(result);
    std::cout << result << " = 10086" << std::endl;
    bthread_join(td2, nullptr);
    bthread_join(td1, nullptr);
    return 0;
}