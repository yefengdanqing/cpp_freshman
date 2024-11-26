#pragma once
#include <string>
#include <iostream>
#include <optional>
#include <iostream>
#include <thread>



template<class T>
class UnitTestClass {
public:
    UnitTestClass<T>(const T& s) : data(s) {
        std::cout << "\tconstruct, thread id:" << std::this_thread::get_id() << std::endl;
    }
    ~UnitTestClass<T>() {
        std::cout << "\tdestruct, thread id:" << std::this_thread::get_id() << std::endl;
    }
    UnitTestClass<T>(const UnitTestClass& unit) {
        data = unit.data;
        std::cout << "\tcopy consturct, thread id:" << std::this_thread::get_id() << std::endl;
    }
    UnitTestClass<T>(UnitTestClass&& unit) {
        data = std::move(unit.data);
        std::cout << "\tmove consturct, thread id:" << std::this_thread::get_id() << std::endl;
    }
    UnitTestClass<T>& operator=(const UnitTestClass<T>& unit) {
        data = unit.data;
        std::cout << "\tcopy operator=, thread id:" << std::this_thread::get_id()  << std::endl;
        return *this;
    }
    UnitTestClass<T>& operator=(UnitTestClass<T>&& unit) {
        data = std::move(unit.data);
        std::cout << "\tmove operator=, thread id:" << std::this_thread::get_id()  << std::endl;
        return *this;
    }
public:
    T data;
};

template<class K>
UnitTestClass<K> get_copy_obj(K value) {
	UnitTestClass<K> unit(value);
    //临时变量，是个右值
    //编译器会做优化的
	return unit; 
}

template<class K>
void copy_obj(UnitTestClass<K> obj) {

}

template<class K>
void OptFunc(std::optional<UnitTestClass<K>> x)
{ 
	std::cout << "\t" << x->data << std::endl;
}

template<class K>
void DefaultFunc(const UnitTestClass<K>& x) 
{ 
	std::cout << "\t" << x.data << std::endl;
}
