#pragma once
#include "dicts.h"
#include "dict/dict_manager.h"
#include "dict/common_file_dict.h"
#include "dict/offer_bid_ratio.h"

namespace utopian {

namespace ranker {
    
// DEFINE_DICT_REGISTER_BASE(OfferBidRatio, offer_bid_ratio);
std::shared_ptr<utopian::ranker::DictManager::DictHander> offer_bid_ratio_info = std::make_shared<utopian::ranker::DictManager::DictHander>();

void register_dict_offer_bid_ratio() {      
if (offer_bid_ratio_info == nullptr) std::cout << "offer_bid_ratio_info ptr is null" << std::endl;   
    utopian::ranker::DictManager::get_single_instance().register_dict<utopian::ranker::OfferBidRatio>("offer_bid_ratio", "./conf/dicts/offer_bid_ratio.txt", offer_bid_ratio_info);    
}

std::shared_ptr<utopian::ranker::DictManager::DictHander> common_dict_info_offer_info = std::make_shared<utopian::ranker::DictManager::DictHander>();
void register_common_dict_info_offer() {
    if (common_dict_info_offer_info == nullptr) std::cout << "common_dict_info_offer_info ptr is null";
    std::cout << "register_common_dict_info_offer" << std::endl;
    utopian::ranker::DictManager::get_single_instance().register_dict<utopian::ranker::CommonFileDict<int64_t, double, std::map>>("common_dict_info_offer_info", "./conf/dicts/offer_bid_ratio.txt", common_dict_info_offer_info);  
}






}

}