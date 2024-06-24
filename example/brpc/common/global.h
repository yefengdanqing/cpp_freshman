#pragma once
#include "common.h"
#include "dict/dict_manager.h"

#include "dicts.h"
#include "dict/offer_bid_ratio.h"



namespace utopian {
namespace ranker {
class GlobalInitializer {
public:
    static GlobalInitializer& get_global_instance() {
        static GlobalInitializer instance;
        return instance;
    }
    int init() {
        register_dict_offer_bid_ratio();
        CHECT_RET(DictManager::get_single_instance().run(true), 0, "init dict manager failed", -1);
        std::shared_ptr<DictBase> offer_bid_ratio_ptr = nullptr;
        // GET_DICT(offer_bid_ratio, offer_bid_ratio_ptr);

        
    }
private:
    GlobalInitializer() {}
    ~GlobalInitializer() {}
    GlobalInitializer(const GlobalInitializer& obj) {}


};

}
}