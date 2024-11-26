#include "babylon/application_context.h"
#include "ranker.h"

using ::babylon::ApplicationContext;

namespace utopian {
namespace ranker{
int RankProcessor::process() noexcept {
    //  *z.emit() = *x + *y;
    RankInfo info;
    info.request_id = normalized_request->request_id;
    info.ecpm = 300;
    info.pivr = 0.01 * prerank_info->pivr;
    *rank_info.emit() = std::move(info);
    return 0;

}
BABYLON_REGISTER_COMPONENT(RankProcessor, "RankProcessor", GraphProcessor);

}
}