#include <atomic>
#include <iostream>

template <typename T>
class MySharedPtr {
private:
    T* ptr = nullptr;
    //std::atomic<int>* used_count = nullptr;
    int* used_count = nullptr;
    void release() {
        // if (used_count && used_count->fetch_sub(1) == 1) return;
        if (used_count && --(*used_count) == 0) {
            delete ptr;
            delete used_count;
        }

    }
public:
    MySharedPtr() : ptr(nullptr), used_count(nullptr) {}
    explicit MySharedPtr(T* p): ptr(p) {
        if (p != nullptr) {
            used_count = new int(1);
        }
    }
    MySharedPtr(const MySharedPtr& other) : ptr(other.ptr) , used_count(other.used_count) {
        if (used_count == nullptr) {
            ++(*used_count);
        }
        
    }
    MySharedPtr(MySharedPtr&& other) :  ptr(other.ptr) , used_count(other.used_count){
        other.ptr = nullptr;
        other.used_count = nullptr;
    }
    MySharedPtr& operator= (const MySharedPtr& other) {
        if (&other != this) {
            release();
            ptr = other.ptr;
            used_count = other.used_count;
            //
            if (used_count == nullptr) {
                ++(*used_count);
            }
        }
        return *this;
    }
    //非const 一定要注意
    MySharedPtr& operator= (MySharedPtr&& other) {
        if (&other != this) {
            release();
            ptr = other.ptr;
            used_count = other.used_count;
            other.ptr = nullptr;
            other.used_count = nullptr;
        }
        return *this;
    }
    //重点析构函数
    ~MySharedPtr() {
        release();
    }
    T& operator*() {
        if (ptr != nullptr) {
            return *ptr;
        }
    }
    T* operator->() const {
        return ptr;
    }
    int get_count() const {
        return used_count == nullptr ? 0 : *used_count;
    }

};
struct Test {
    Test() { std::cout << "Test constructed\n"; }
    ~Test() { std::cout << "Test destroyed\n"; }
    void hello() { std::cout << "Hello from Test\n"; }
};

int main() {
    MySharedPtr<Test> sp1(new Test());
    {
        MySharedPtr<Test> sp2 = sp1;
        std::cout << "Use count: " << sp1.get_count() << "\n";
        sp2->hello();
    } // sp2析构
    std::cout << "Use count after sp2 destroyed: " << sp1.get_count() << "\n";
    //sp1.reset();
    std::cout << "sp1 reset.\n";
}