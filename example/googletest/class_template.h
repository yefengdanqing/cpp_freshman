#pragma once
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>


template <typename K, typename V, template<typename, typename> typename C>
class CommonFileDict {
public:
    CommonFileDict() {
        std::cout << "泛化构造函数1:" << typeid(*this).name() << std::endl;
    }
    CommonFileDict(const std::string& file_name) {
        std::cout << "泛化构造函数2:" << typeid(*this).name() << std::endl;
        _dict_file = file_name;
    }

    bool is_exist(const K& key);
    void insert(const K& key, const V& value);
    void add(const K& key, const V& value) {
        auto iter = kv_container.find(key);
        if (iter == kv_container.end()) {
            kv_container[key] = value;
        } else {
            iter->second = iter->second + value;
        }
    }
    void function_print(const K& key, const V& value)
    {
      std::cout << "泛化的3function_print:" << typeid(*this).name() << " " << typeid(key).name() << " " << typeid(value).name() << " " << typeid(kv_container).name() << std::endl;  
    }

    const V get_value(const K& key) {
        if (auto it = kv_container.find(key); it != kv_container.end()) {
            return it->second;
        } else {
            return V();
        }
    }

private:
    int count;
    std::string name;
    std::string _dict_file;
    C<K, V> kv_container;
};


template<typename K, typename V, template<typename, typename> typename C>
void CommonFileDict<K, V, C>::insert(const K& key, const V& value) {
    kv_container[key] = value;

}

template<typename K, typename V, template<typename, typename> typename C>
bool CommonFileDict<K, V, C>::is_exist(const K& key) {
    return kv_container.find(key) != kv_container.end();
}


//全特化
template<>
class CommonFileDict<std::string, int, std::map> {
public:
    void insert(const std::string& key, const int& value);
    void add(const std::string& key, const int& value);


private:
    int count;
    std::string name;
    std::string _dict_file;
    std::map<std::string, int> kv_container;

};


//偏特化
template<typename V>
class CommonFileDict<std::string, V, std::map> {
    public:
    void insert(const std::string& key, const V& value) {}
    void add(const std::string& key, const V& value) {
        auto iter = kv_container.find(key);
        if (iter == kv_container.end()) {
            kv_container[key] = value;
        } else {
            iter->second = iter->second + value;
        }
    }
private:
    int count;
    std::string name;
    std::string _dict_file;
    std::map<std::string, V> kv_container;
};

//需要声明,在源文件中单独声明的时候需要定义
//为啥，待学习
// template<>
// void CommonFileDict<std::string, double, std::map>::function_print(const std::string& key, const double& value);


