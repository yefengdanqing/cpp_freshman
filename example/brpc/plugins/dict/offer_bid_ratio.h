#pragma once
#include <map>

#include "file_dict.h"

namespace utopian {
namespace ranker {

class OfferBidRatio : public FileDict {
public:
    bool init() override;
    bool load() override;
    OfferBidRatio(const std::string& file) {
        _dict_file = file;
    }

    const std::string get_cur_file() override {
        return _dict_file;
    }
    double get_key(int64_t key) {
        if (auto bid_iter = offer_exp_ratio.find(key); bid_iter != offer_exp_ratio.end()) {
            return bid_iter->second;
        } else {
            return 0;
        }
    }
private:
    std::map<int64_t, double> offer_exp_ratio;
};


}
}