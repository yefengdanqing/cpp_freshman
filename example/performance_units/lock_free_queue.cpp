#include <string>
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>


struct ListNode
{
    std::string log_info;
    ListNode* next = nullptr;
    ListNode() {}
    ListNode(const std::string& data) : log_info(data) {

    }
};


class LockFreeQueue {
private:
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
public:
    LockFreeQueue() {
        // head = new ListNode();
        // tail = new ListNode();
        head = new ListNode(); // 空值哨兵节点
        tail = head;           // 一开始 tail 和 head 都指向同一个空节点
    }
    bool input(const std::string& data) {
        ListNode* node = new ListNode(data);
        ListNode* cur = nullptr;
        while (true) {
            cur = tail;
            //ListNode* next = cur->next;
            if (cur->next == nullptr) {
                if (__sync_bool_compare_and_swap(&cur->next, nullptr, node)) {
                    break;
                }
            } else {
                // tail 落后了，尝试推进tail
                __sync_bool_compare_and_swap(&tail, cur, cur->next);
            }
        }
        // 推进 tail 指向新插入节点
        __sync_bool_compare_and_swap(&tail, cur, node);
        return true;
    }
    std::string output() {
        ListNode* first = nullptr;
         ListNode* next = nullptr;
        while (true) {
            first = head;
            next = first->next;

            // 队列为空
            if (next == nullptr) {
                return "";
            }

            // 尝试原子地将 head 向后移动
            if (__sync_bool_compare_and_swap(&head, first, next)) {
                std::string result = next->log_info;
                //delete first;  // 删除旧的哨兵节点
                return result;
            }

            // 如果 CAS 失败，说明有其他线程抢先弹出，再试一次
        }

    }

};
// std::mutex mt;
// std::queue<int> qe;
// std::unique_lock<std::mutex> guard(mt);
// std::condition_variable cv;
// cv.wait(guard, []{return qe.empty();});
int main() {

     {
        char a[10] = {0, 1,2,3,4,5,6,7};
        std::cout << a << " " << a+1 << " " << a+2 << " " << a+3 << " " << a + 4 << " " << a+ 5 << " " << a+ 6 << " " << a+ 7 << std::endl; 
        int* p = (int*)a;
        std::cout << p <<"--" << p++ << "--" << p << "--" <<(short int*)(p) << std::endl;
        std::cout << *(short int*)(p) << std::endl;
    }
    LockFreeQueue log_test;
    std::vector<std::thread> threads;
    const int produce_count = 10;
    const int consume_count = 20;
    for (int i = 0; i < produce_count; i++) {
        threads.emplace_back([&log_test](){
            int i = 1;

            while (1) {
                std::string tmp = "datata--" + std::to_string(i);
                std::cout << "produce:" << tmp << std::endl;
                log_test.input(tmp);
                i++;
            }
        });
    }

    for (int i = 0; i < consume_count; i++) {
        threads.emplace_back([&log_test](){
            int i = 0;

            while (1) {
                std::cout << "consume:" << log_test.output() << std::endl;
            }
        });
    }

    return 0;

}