#include <string>
#include <iostream>
#include <iostream>
#include <utility>

class Base {
private:
    int a = 3;
public:
    virtual void print(int n = 3) {
        std::cout << "base:n+a" << n+a << std::endl;
    }
};
class Deriver : public Base {
private:
    int b = 4;
public:
    virtual void print(int n =10) {
        std::cout << "deriver:n+a" << n+b << std::endl;
    }

};
template <typename T> 
void wrapper(T&& val){
    print(std::forward<T>(val));
}


void print(int& x)  { std::cout << "LValue,x=" << x << std::endl; }
void print(int&& x) { std::cout << "RValue,x=" << x << std::endl; }
int main() {
    Base* arr = new Deriver[10];
    arr[7].print();
    delete[] arr;
    
    Base* ptr = new Deriver();
    ptr->print();
    delete ptr;

    wrapper(10);
    int val = 42;
    wrapper(val);

    int& m = val;
    wrapper(m);
    wrapper(static_cast<int&>(m));

    int&& n = 1000;
    wrapper(n);
    wrapper(static_cast<int&&>(n));

    return 0;
}