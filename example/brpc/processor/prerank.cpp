#include "babylon/application_context.h"

#include "prerank.h"

using ::babylon::ApplicationContext;



namespace utopian {
namespace ranker{
int PrerankProcessor::process() noexcept {
    //  *z.emit() = *x + *y;
    PrerankInfo info;
    info.ecpm = 99999;
    info.offer_id = 11111;
    info.pivr = 0.02;
    info.request_id = normalized_request->request_id;
    *prerank_info.emit() = std::move(info);
        
}
// BABYLON_REGISTER_FACTORY_COMPONENT(PrerankProcessor, "PrerankProcessor", GraphProcessor);

}
}