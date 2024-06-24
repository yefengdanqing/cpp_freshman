#include <thread>

#include "butil/logging.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "gflags/gflags.h"
#include "bthread/bthread.h"

#include "dict_base.h"
#include "dict_manager.h"

DEFINE_string(dict_path, "./conf/dict/", "config dict path");
DEFINE_string(dict_file, "dict_config.xml", "file path");

namespace utopian {
namespace ranker {
void DictManager::initialize() noexcept {
    boost::property_tree::ptree pt;
    std::ifstream xml_file("config.xml"); 
    boost::property_tree::read_xml(xml_file, pt);
    for (const auto& dict : pt.get_child("dicts")) {
        if (dict.first == "dict") {
            DictConfig conf;
            conf.file_path = dict.second.get<std::string>("file");
            conf.dict_name = dict.second.get<std::string>("name");
            conf.generator_name = dict.second.get<std::string>("type");
            dict_config_vect.emplace_back(std::move(conf));
        }
    }
}
int32_t DictManager::run(bool is_wait_all_done) {
    // 并行加载
    int32_t ret = parallel_sync_load();
    try {
        //线程时刻check
        std::thread t(std::bind(&DictManager::check, this));
        auto pid = t.get_id();
        t.detach();
        LOG(NOTICE) << "reload dict thread pid=" << pid;
    } catch(std::system_error& err) {
        LOG(WARNING) << "start dict thread fail error reason=" << err.what();
        return -1;
    }
    return 0;
}
void DictManager::check() {
    int sleep_time = 60;
    while (1) {
        for (auto& dict : dict_hander_vect) {
            if (dict == nullptr)    continue;
            time_t now = time(NULL);
            LOG(NOTICE) << "start check time" << now;
            int cur_idx = dict->cur_index.load();
            int bg_idx = 1 - cur_idx;
            auto& cur_dict = dict->dicts[cur_idx];
            if (cur_dict == nullptr) {
                LOG(NOTICE) << "cur dict is nullptr";
                continue;
            }
            auto& bg_dict = dict->dicts[bg_idx];
            if (bg_dict != nullptr && abs(now - dict->load_finish_time > 120)) {
                // 加载完成时间超过2min, 删除旧词典
                LOG(NOTICE) << "free dict : " << bg_dict->get_cur_file() << " success" << ", the effective time is : " << now;
                bg_dict.reset();
            }
            time_t new_mtime = get_file_last_mtime(cur_dict->get_cur_file());
            if (new_mtime == dict->modify_time) {
                LOG(NOTICE) << "dict:" << cur_dict->get_cur_file() << ", time:" << new_mtime;
                continue;
            }
            if (bg_dict != nullptr) {
                bg_dict.reset();
            }

            auto new_dict = dict->cfunc();
            if (new_dict->init() == 0 && 
                new_dict->load()) {
                bg_dict = new_dict;
                dict->modify_time = new_mtime;
                dict->load_finish_time = time(NULL);
                LOG(NOTICE) << "dict:" << cur_dict->get_cur_file() << ", time" << new_mtime << ", modify_time:" << dict->modify_time << " ,cur_index:" << dict->cur_index << " bg_idx:" << bg_idx;
                dict->cur_index = bg_idx;
            }
        }
        sleep(sleep_time);
    }

}

void* DictManager::load_dict_by_thread(void* args) {
    auto load_args = (DictLoadArg*)args;
    std::shared_ptr<DictManager::DictHander> dict_ptr = load_args->dict_ptr;
    int index = dict_ptr->cur_index.load();
    auto cur_one = dict_ptr->dicts[index];
    if (cur_one == nullptr) {
        load_args->ret = -1;
        LOG(NOTICE) << "dicts " << dict_ptr->dict_name << ", index:" << index;
        return nullptr;
    }
    dict_ptr->modify_time = get_file_last_mtime(cur_one->get_cur_file());
    if (cur_one != nullptr && cur_one->load()) {
        LOG(NOTICE) << "load dict : parallel sync load dict[" << dict_ptr->dict_name << "] path["
            << cur_one->get_cur_file() << "] success" << ", the effective time is : " << dict_ptr->modify_time;
        load_args->ret = 0;
    } else {
        /*LOG(NOTICE) << "load dict : parallel sync load dict[" << dict_ptr->dict_name << "] path["
            << cur_one->get_cur_file() << "] failed, the failure time is : " << dict_ptr->modify_time;*/
        LOG(NOTICE) << "load dict : parallel sync load dict[" << dict_ptr->dict_name << "] path[] failed, the failure time is : " << dict_ptr->modify_time;
        load_args->ret = -1;
    }
}

int32_t DictManager::parallel_sync_load() {
    if (dict_hander_vect.empty()) {
        return 0;
    }

    std::vector<DictLoadArg> load_args;
    load_args.resize(dict_hander_vect.size());


    int index = 0;
    for(auto& info : dict_hander_vect) {
        if (info == nullptr)    continue;
        int idx = info->cur_index.load();
        auto& hander_ptr = info->dicts[idx];
        //必须先new一个
        hander_ptr = info->cfunc();
        if (hander_ptr == nullptr) {
            LOG(INFO) << "dict name:" << info->dict_name << "is null";
            continue;
        }
        hander_ptr->init();
        auto& args = load_args[index++];
        args.dict_ptr = info;
        args.ret = bthread_start_background(&args.id, NULL, DictManager::load_dict_by_thread, (void*)(&args));

        if (args.ret != 0) {
            LOG(FATAL) << "create bthread error for load_dict_by_bthread dict:" << info->dict_name;
            continue;
        }
    }
    for(const auto& load_result : load_args) {
        bthread_join(load_result.id, nullptr);
        if (load_result.ret != 0) {
            LOG(FATAL) << "load dict " << load_result.dict_ptr->dict_name << "failed";
            return -1;
        }
    }
    return 0;
}


std::shared_ptr<DictBase> DictManager::get_dict_info(const std::string& name) {
    // if (auto it = dict_map.find(name); it != dict_map.end()) {
    //     int idx = it->second->cur_index.load();
    //     return it->second->dicts[idx];
    // } else {
    //     return nullptr;
    // }

}



}
}