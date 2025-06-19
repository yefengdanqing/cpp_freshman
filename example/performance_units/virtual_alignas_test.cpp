#include <string>
#include <iostream>

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
int main() {
    Base* arr = new Deriver[10];
    arr[7].print();
    delete[] arr;
    
    Base* ptr = new Deriver();
    ptr->print();
    delete ptr;
    return 0;
}