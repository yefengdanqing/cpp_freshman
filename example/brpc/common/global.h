#pragma once

#include "butil/logging.h"

#include "common.h"
#include "dict/dict_manager.h"

#include "dicts.h"
#include "dict/offer_bid_ratio.h"
#include "dict/common_file_dict.h"



namespace utopian {
namespace ranker {
class GlobalInitializer {
public:
    static GlobalInitializer& get_global_instance() {
        static GlobalInitializer instance;
        return instance;
    }
    int init() {
        //调用要在前面

        CHECT_RET(DictManager::get_single_instance().run(true), 0, "init dict manager failed", -1);

        std::shared_ptr<OfferBidRatio> offer_bid_ratio_ptr = nullptr;
        GET_DICT(offer_bid_ratio, offer_bid_ratio_ptr, OfferBidRatio);
        if (offer_bid_ratio_ptr != nullptr) {
            double val = offer_bid_ratio_ptr->get_key(311111111);
            LOG(WARNING) << "offer_bid_ratio_ptr get key:" << val;
        }

        std::shared_ptr<CommonFileDict<int64_t, double, std::map>> common_dict_info_offer_ptr = nullptr;
        GET_DICT(common_dict_info_offer, common_dict_info_offer_ptr, WRAP(CommonFileDict<int64_t, double, std::map>));
        if (common_dict_info_offer_ptr != nullptr) {
            double val = common_dict_info_offer_ptr->get_key(311111111);
            LOG(WARNING) << "common_dict_info_offer get key " << val;
        }

        return 0;

        
    }
private:
    GlobalInitializer() {}
    ~GlobalInitializer() {}
    GlobalInitializer(const GlobalInitializer& obj) {}


};

}
}