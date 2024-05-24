#include "butil/logging.h"
#include "output.h"

namespace ranker {
namespace ranker{

int OutputProcessor::process() noexcept {
    //  *z.emit() = *x + *y;
    ResponseInfo info;
    info.bid_price = normalized_request->price * prerank_info->pivr * rank_info->pivr * 1000 * prerank_info->ecpm;
    info.request_id = normalized_request->request_id;
    info.timeout = normalized_request->timeout;
    LOG(INFO) << "price:" << normalized_request->price << ",request_id" << info.request_id << ", bid_price:" << info.bid_price << ", pre_pivr:" << prerank_info->pivr << ",rank_pivr" << rank_info->pivr;
    *response_info.emit() = std::move(info);     
}
}
}