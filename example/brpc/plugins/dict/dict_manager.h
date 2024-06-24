#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <memory>
#include <atomic>
#include <string_view>

#include "bthread/bthread.h"

// extern std::shared_ptr<utopian::ranker::DictManager::DictHander> dict_name_##info;
#define GET_DICT(dict_name, dict_ret)  \
    extern std::shared_ptr<utopian::ranker::DictManager::DictHander> dict_name_##info;\
    dict_ret = dict_name_##info->dicts[dict_name_##info->cur_index.load()]; 


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

typedef std::shared_ptr<DictBase> (*create_func)();
// using create_func = std::shared_ptr<DictBase> (*)();

template<typename T>
std::shared_ptr<DictBase> __create_func() {
    return std::make_shared<T>();
}

// 注册函数，将模板函数实例化后赋值给函数指针
template<typename T>
create_func register_func() {
    return &__create_func<T>;
}





class DictManager {
public:
    struct DictConfig {
        std::string file_path;
        std::string dict_name;
        std::string generator_name;
    };

    struct DictHander {
        std::string dict_name;
        std::shared_ptr<DictBase> dicts[2];
        std::atomic<int> cur_index;
        create_func cfunc = nullptr;
        time_t modify_time;
        time_t load_finish_time;
    };
public:
    static DictManager& get_single_instance() {
        static DictManager instance;
        return instance;
    }
    //dict_name 不能用引用么
    template<typename T>
    void register_dict(std::string dict_name,
                        std::shared_ptr<DictHander> info) {
        dict_hander_vect.emplace_back(info);
        if (info == nullptr) {
            //日志
            std::cout << "ptr is null" << std::endl;
            return;
        }
        info->dict_name = dict_name;
        info->modify_time = 0;
        info->cfunc = (register_func<T>());
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

#define DEFINE_DICT_REGISTER_BASE(class_name, dict_name) \
    std::shared_ptr<utopian::ranker::DictManager::DictHander> dict_name_##info = std::make_shared<utopian::ranker::DictManager::DictHander>();    \
    void register_dict_##dict_name() {      \
        if (dict_name_##info == nullptr) std::cout << "ptr is null" << std::endl;   \
        utopian::ranker::DictManager::get_single_instance().register_dict<utopian::ranker::class_name>(#dict_name, dict_name_##info);    \
    }   \
    void register_dict_##dict_name() __attribute__((constructor));

}
}
