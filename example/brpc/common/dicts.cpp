#pragma once
#include "dicts.h"
#include "dict/dict_manager.h"
#include "dict/common_file_dict.h"
#include "dict/offer_bid_ratio.h"

namespace utopian {

namespace ranker {


DEFINE_DICT_REGISTER_BASE(OfferBidRatio, "./conf/dicts/offer_bid_ratio.txt", offer_bid_ratio);
DEFINE_DICT_REGISTER_BASE(WRAP(CommonFileDict<int64_t, double, std::map>), "./conf/dicts/offer_bid_ratio.txt", common_dict_info_offer);

// DEFINE_DICT_REGISTER_BASE(WRAP(CommonFileDict<int64_t, std::string, std::map>), "./conf/dicts/index_pkg_tag_info.txt", index_pkg_tag_info);







}

}