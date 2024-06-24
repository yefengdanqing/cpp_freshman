#pragma once
#include "dicts.h"
#include "dict/dict_manager.h"
#include "dict/offer_bid_ratio.h"

namespace utopian {

namespace ranker {

// DEFINE_DICT_REGISTER_BASE(OfferBidRatio, offer_bid_ratio);
std::shared_ptr<utopian::ranker::DictManager::DictHander> offer_bid_ratio_info = std::make_shared<utopian::ranker::DictManager::DictHander>();    
void register_dict_offer_bid_ratio() {      
        if (offer_bid_ratio_info == nullptr) std::cout << "ptr is null" << std::endl;   
        utopian::ranker::DictManager::get_single_instance().register_dict<utopian::ranker::OfferBidRatio>("offer_bid_ratio", offer_bid_ratio_info);    
}   





}

}