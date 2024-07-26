#pragma once
#include <string>
#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <utility>

#include <boost/type_index.hpp>
class Base {};
class Derived : public Base {};
class Unrelated {};

template <typename T>
void print_type_name() {
    std::cout << "Type: " << boost::typeindex::type_id_with_cvr<T>().pretty_name() << std::endl;
}

//函数模板的重载,通过条件限制类型
template <typename T>
typename std::enable_if<std::is_same<T, std::string>::value, T>::type add (T a, T b) {
    T result = a + " " + b;
    return result;
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type add(T a, T b) {
    std::cout << "type:" << typeid(a).name() << " " << typeid(b).name()<< std::endl;
    print_type_name<T>();
    T result = a + b;
    return result;
}

template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type add(T a, T b) {
    std::cout << "type:" << typeid(a).name() << " " << typeid(b).name()<< std::endl;
    print_type_name<double>();
    double result = a * 100 + b * 100;
    return result;
}
//注意函数覆盖的方式

//泛化和特化,特化是相对于模板类型的某个特定类型的详细定义
template<typename T>
T decrease(T a, T b) {
    std::cout << "type:" << typeid(a).name() << " " << typeid(b).name()<< std::endl;
    return a - b;
}

//函数的全特化版本
template<>
std::string decrease(std::string a, std::string b) {
    std::cout << "type:" << typeid(a).name() << " " << typeid(b).name()<< std::endl;
    return b + " " + a;
}

template<typename T>
int foo() {
    T a;
    return 10;  
}
// template<>
// int foo() {
//     int a = 10;
//     return 10;
// }

template<>
int foo<int>() {
    int a = 10;
    return 10;
}