#pragma once
#include <map>

#include "file_dict.h"

namespace utopian {
namespace ranker {

class OfferBidRatio : public FileDict {
public:
    bool init() override;
    bool load() override;

    const std::string get_cur_file() override {
        return _dict_file;
    }
private:
    std::map<int64_t, double> offer_exp_ratio;
};


}
}