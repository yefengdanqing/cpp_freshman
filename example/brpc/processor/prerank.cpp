#include <typeinfo>

#include "babylon/application_context.h"

#include "prerank.h"

using ::babylon::ApplicationContext;



namespace utopian {
namespace ranker{
int PrerankProcessor::process() noexcept {
    PrerankInfo info;
    bool flag = (*IsSkipPreRank);
    BABYLON_LOG(INFO) << "PrerankProcessor:" << int(flag) << " " << typeid(IsSkipPreRank).name();
    if (*IsSkipPreRank) {
        BABYLON_LOG(INFO) << "PrerankProcessor::is_skip";
        *prerank_info.emit() = std::move(info);
        return 0;
    } else {
        BABYLON_LOG(INFO) << "PrerankProcessor::is_not_skip";
    }
    info.ecpm = 99999;
    info.offer_id = 11111;
    info.pivr = 0.02;
    info.request_id = normalized_request->request_id;
    *prerank_info.emit() = std::move(info);
    BABYLON_LOG(INFO) << "PrerankProcessor::end";
    return 0;
        
}
BABYLON_REGISTER_COMPONENT(PrerankProcessor, "PrerankProcessor", GraphProcessor);

}
}