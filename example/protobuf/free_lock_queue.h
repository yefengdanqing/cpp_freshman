#include <string>

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x) {}
};

class FreeLockQueue {
private:
    ListNode* tail;
    ListNode* head;
    int capacity;
public:
    bool en_queue(int x) {
        ListNode* cur = nullptr;
        auto node = new ListNode(x);
        while (1) {
            cur = tail;
            //当前线程更新了节点
            if (__sync_bool_compare_and_swap(&(cur->next), nullptr, node)) {
                break;
            } else {
                // 说明有其他节点更新了尾节点,同步更新
                __sync_bool_compare_and_swap(&tail, cur, cur->next);
            }
        }
        __sync_bool_compare_and_swap(&tail, cur, node);
        return true;
    }
    int de_queue() {
        ListNode* cur = nullptr;
        while(1) {
            //每次都是获取最新的
            cur = head;
            //队列为空
            if (cur->next == nullptr) {
                return -1;
            }
            //保证不是别的线程出队了数据
            __sync_bool_compare_and_swap(&head, cur, cur->next);
            int val =cur ->next->val;
            delete cur;
            return val;
        }

        

    }
};