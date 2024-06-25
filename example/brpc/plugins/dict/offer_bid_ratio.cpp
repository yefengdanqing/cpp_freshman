#include <fstream>
#include <sys/stat.h>
#include <string_view>


#include "butil/logging.h"

#include "util/split_str.h"
#include "offer_bid_ratio.h"

namespace utopian {

namespace ranker {
bool OfferBidRatio::init() {
    // _dict_file = "./conf/dicts/offer_bid_ratio.txt";
    return true;

}
bool OfferBidRatio::load() {
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
    std::vector<double> items;
    items.resize(2);
    while (!input.eof()) {

        std::getline(input, line);
        if (line.size() == 0) {
            continue;
        }
        items.clear();
        split_new_v1<double>(line, "\t", items);
        if (items.size() < 2)   continue;
        offer_exp_ratio.emplace(int64_t(items[0]), items[1]);
    }
    LOG(NOTICE) << "dict path:" << _dict_file << ", load numbers:" << offer_exp_ratio.size();
    return true;
}

}
}