#pragma once
#include "common.h"
#include "dict/dict_manager.h"

namespace utopian {

namespace ranker {

// DECLEAR_DICT_BASE(offer_bid_ratio);

extern std::shared_ptr<utopian::ranker::DictManager::DictHander> offer_bid_ratio_info;
void register_dict_offer_bid_ratio();




}

}