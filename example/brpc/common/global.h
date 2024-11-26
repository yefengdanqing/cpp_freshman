#pragma once

#include "butil/logging.h"
#include "babylon/application_context.h"

#include "common.h"
#include "dict/dict_manager.h"

#include "dicts.h"
#include "dict/offer_bid_ratio.h"
#include "dict/common_file_dict.h"

 
using ::babylon::ApplicationContext;



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
        //可以把所有的类型组合定义出来
        std::shared_ptr<CommonFileDict<int64_t, double, std::map>> common_dict_info_offer_ptr = nullptr;
        GET_DICT(common_dict_info_offer, common_dict_info_offer_ptr, WRAP(CommonFileDict<int64_t, double, std::map>));
        if (common_dict_info_offer_ptr != nullptr) {
            double val = common_dict_info_offer_ptr->get_key(311111111);
            LOG(WARNING) << "common_dict_info_offer get key " << val << ",not key:" << common_dict_info_offer_ptr->get_key(4444444);
        }
        // std::shared_ptr<CommonFileDict<int64_t, std::string, std::map>> index_pkg_tag_info_ptr;
        // GET_DICT(index_pkg_tag_info, index_pkg_tag_info_ptr, WRAP(CommonFileDict<int64_t, std::string, std::map>));
        // if (index_pkg_tag_info_ptr != nullptr) {
        //     const std::string& val = index_pkg_tag_info_ptr->get_key(1);
        //     LOG(WARNING) << "common_dict_info_offer get key " << val << ",not key:" << index_pkg_tag_info_ptr->get_key(2);
        // }

        auto graph_engine = ApplicationContext::instance().get<framework::GraphEngine>("graph_engine");
        if (graph_engine) {
            LOG(WARNING) << "graph engine is not null";
            graph_engine->initialize();
            graph_engine->init_pool(20, 20, "main_graph");

            auto graph = graph_engine->try_get("main_graph");
            if (graph == nullptr) {
                LOG(WARNING) << "graph engine is not null";
            }


        } else {
            LOG(WARNING) << "graph engine is null";
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