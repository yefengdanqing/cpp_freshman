#pragma once

#include "babylon/anyflow/builder.h"
#include "babylon/anyflow/graph.h"

#include "common/common.h"
#include "demo_proto.pb.h"


using babylon::anyflow::Graph;
using babylon::anyflow::GraphData;
using babylon::anyflow::Closure;
using babylon::anyflow::GraphBuilder;
using babylon::anyflow::GraphProcessor;
using babylon::anyflow::Committer;
using ranker_server::RsResponse;

namespace utopian {
namespace ranker {

class OutputProcessor : public GraphProcessor {
    //int setup(GraphVertex& vertex) noexcept;
private:
    int process() noexcept override;
    ANYFLOW_INTERFACE (
        ANYFLOW_DEPEND_DATA(RequestInfo, normalized_request)
        ANYFLOW_DEPEND_DATA(PrerankInfo, prerank_info)
        ANYFLOW_DEPEND_DATA(RankInfo, rank_info)
        ANYFLOW_EMIT_DATA(ResponseInfo, response_info)
        ANYFLOW_EMIT_DATA(RsResponse, rs_response)
    )

};
    
}
}