#pragma once
#include <fstream>
#include <sys/stat.h>
#include <string_view>


#include "util/split_str.h"
#include "file_dict.h"

namespace utopian {
namespace ranker {

template <typename K, typename V, template<typename, typename> typename C>
class CommonFileDict : public FileDict {
public:
    CommonFileDict() {
    }
    CommonFileDict(const std::string& file) {
        _dict_file = file;
    }
    bool init() override {
        // _dict_file = "./conf/dicts/offer_bid_ratio.txt";
        return true;
    }
    bool load() override {
        struct stat sb;
        if ((stat(_dict_file.c_str(), &sb) == -1) || ((sb.st_mode & S_IFMT) != S_IFREG)) {
            LOG(WARNING) << "dict file error:" << _dict_file;
            return false;
        }

        std::string strtemp;
        std::ifstream input(_dict_file.c_str(), std::ifstream::in);
        if (!input.good()) {
            input.close();
            LOG(WARNING) << _dict_file << " dict input file error!";
            return false;
        }
        std::string line;
        std::vector<std::string> items;
        items.resize(2);
        while (!input.eof()) {

            std::getline(input, line);
            if (line.size() == 0) {
                continue;
            }

            items.clear();
            split_new_v1<std::string>(line, "\t", items);
            if (items.size() < 2)   continue;
            K key = boost::lexical_cast<K>(items[0]);
            V value = boost::lexical_cast<V>(items[1]);
            //先无脑用覆盖的方式
            key_value_mapping[key] = value;
        }
        LOG(NOTICE) << "dict path:" << _dict_file << ", load numbers:" << key_value_mapping.size();
        return true;

    }
    V get_key(const K& key) {
        if (auto it = key_value_mapping.find(key); it != key_value_mapping.end()) {
            return it->second;
        } else {
            return V();
        }
    }

    const std::string get_cur_file() {
        return _dict_file;
    }
    virtual ~CommonFileDict() {}
private:
    C<K, V> key_value_mapping;
};


}
}

