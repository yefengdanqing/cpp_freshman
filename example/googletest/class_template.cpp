#include "class_template.h"



void CommonFileDict<std::string, int, std::map>::add(const std::string& key, const int& value) {
        auto iter = kv_container.find(key);
        if (iter == kv_container.end()) {
            kv_container[key] = value;
        } else {
            iter->second = iter->second + value;
        }
}



void CommonFileDict<std::string, int, std::map>::insert(const std::string& key, const int& value) {
    kv_container[key] = value;
}

// template<>
// void CommonFileDict<std::string, double, std::map>::function_print(const std::string& key, const double& value) {
//     std::cout << "泛化的function_print4:" << typeid(*this).name()
//               << " " << typeid(key).name()
//               << " " << typeid(value).name()
//               << " " << typeid(kv_container).name() << std::endl;
// }