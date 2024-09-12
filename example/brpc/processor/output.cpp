#include "butil/logging.h"
#include "babylon/application_context.h"
using ::babylon::ApplicationContext;
#include "output.h"

namespace utopian {
namespace ranker{

int OutputProcessor::process() noexcept {
    //  *z.emit() = *x + *y;
    ResponseInfo info;
    info.bid_price = normalized_request->price * prerank_info->pivr * rank_info->pivr * 1000 * prerank_info->ecpm;
    info.request_id = normalized_request->request_id;
    info.timeout = normalized_request->timeout;
    LOG(INFO) << "price:" << normalized_request->price << ",request_id" << info.request_id << ", bid_price:" << info.bid_price << ", pre_pivr:" << prerank_info->pivr << ",rank_pivr" << rank_info->pivr;
    *response_info.emit() = std::move(info);

    auto response_ptr = rs_response.emit();

    Committer<RsResponse> committer = rs_response.emit();
    if (committer.valid()) {
        LOG(INFO) << "valid";
    } else {
        LOG(INFO) << "invalid";
    }

    response_ptr->set_request_id(normalized_request->request_id);
    response_ptr->set_error_msg("success");
    response_ptr->set_ext_algo_list("ext_algo");
    return 0;
}

// BABYLON_REGISTER_FACTORY_COMPONENT(OutputProcessor, "OutputProcessor", GraphProcessor);

}
}