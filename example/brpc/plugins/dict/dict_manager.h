#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <memory>
#include <atomic>
#include <string_view>
#include <functional>

#include "bthread/bthread.h"

// extern std::shared_ptr<utopian::ranker::DictManager::DictHander> dict_name##_info;
// static_pointer_cast和dynamic_pointer_cast
#define WRAP(...) __VA_ARGS__
#define GET_DICT(dict_name, dict_ret, class_type)  \
    extern std::shared_ptr<utopian::ranker::DictManager::DictHander> dict_name##_info;\
    dict_ret = std::static_pointer_cast<class_type>(dict_name##_info->dicts[dict_name##_info->cur_index.load()]);


namespace utopian {
namespace ranker {
class DictBase;


inline static std::time_t get_file_last_mtime(const std::string& file) {
    if (file.empty() || !std::filesystem::exists(file)) {
        return 0;
    }
    auto f_time = std::filesystem::last_write_time(file);
    return decltype(f_time)::clock::to_time_t(f_time);
}
template<typename T>
std::shared_ptr<DictBase> __create_func(const std::string& file_path) {
    return std::make_shared<T>(file_path);
}

using create_func = std::function<std::shared_ptr<DictBase>()>;

// using create_func = std::function<std::shared_ptr<DictBase>(const std::string&)>;
// typedef std::shared_ptr<DictBase> (*create_func)(const std::string&);
// using create_func = std::shared_ptr<DictBase> (*)(const std::string&);
// 注册函数，将模板函数实例化后赋值给函数指针
// template<typename T>
// create_func register_func(const std::string& path) {
//     return &__create_func<T>;
// }


class DictManager {
public:
    struct DictConfig {
        std::string file_path;
        std::string dict_name;
        std::string generator_name;
    };

    struct DictHander {
        std::string dict_name;
        std::string dict_file_path; //记录
        std::shared_ptr<DictBase> dicts[2];
        std::atomic<int> cur_index;
        create_func cfunc;
        time_t modify_time;
        time_t load_finish_time;
    };
public:
    static DictManager& get_single_instance() {
        static DictManager instance;
        return instance;
    }
    template<typename T>
    void register_dict(const std::string& dict_name,
                        const std::string& dict_file_path,
                        std::shared_ptr<DictHander> info) {
        if (info == nullptr) {
            //日志
            LOG(WARNING) << "ptr is null, dict name:" << dict_name;
            return;
        }

        dict_hander_vect.emplace_back(info);
        info->dict_name = dict_name;
        info->dict_file_path = dict_file_path;
        info->modify_time = 0;
        //这是调用
        // info->cfunc = __create_func<T>(dict_file_path);
        //在调用的地方传入
        // info->cfunc = __create_func<T>; 
        //可以用bind，或者lambda
        info->cfunc = std::bind(__create_func<T>, dict_file_path);
    }
    static void* load_dict_by_thread(void* ptr);
    int32_t run(bool is_wait_all_done = false);
    virtual void initialize() noexcept;
    std::shared_ptr<DictBase> get_dict_info(const std::string& name);

private:
    std::unordered_map<std::string, std::shared_ptr<DictHander>> dict_map;
    std::vector<DictConfig> dict_config_vect;   //从配置里面维护的词典配置
    std::vector<std::shared_ptr<DictHander>> dict_hander_vect;   //hander
private:
    DictManager() {}
    DictManager(const DictManager& obj) {}
    ~DictManager() {}
    int32_t parallel_sync_load();  //并行加载
    void check();   // 周期性的通过mtime检查词表是否有更新
    
};

struct DictLoadArg {
    bthread_t id;
    std::shared_ptr<DictManager::DictHander> dict_ptr;
    int ret;
};


#define DEFINE_DICT_REGISTER_BASE(class_name, dict_file_path, dict_name) \
    std::shared_ptr<utopian::ranker::DictManager::DictHander> dict_name##_info = nullptr;    \
    void register_dict_##dict_name() {      \
        dict_name##_info = std::make_shared<utopian::ranker::DictManager::DictHander>(); \
        if (dict_name##_info == nullptr) std::cout << #dict_name "_info ptr is null" << std::endl;   \
        else std::cout << #dict_name "_info ptr is not null" << std::endl;   \
        utopian::ranker::DictManager::get_single_instance().register_dict<utopian::ranker::class_name>(#dict_name, dict_file_path, dict_name##_info);    \
    }   \
    void register_dict_##dict_name() __attribute__((constructor));


#define DECLEAR_DICT_BASE(dict_name)    \
    extern std::shared_ptr<utopian::ranker::DictManager::DictHander> dict_name##_info;

}
}
