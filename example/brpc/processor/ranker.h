#pragma once

#include "babylon/anyflow/builder.h"
#include "babylon/anyflow/graph.h"

#include "common/common.h"

using babylon::anyflow::Graph;
using babylon::anyflow::GraphData;
using babylon::anyflow::Closure;
using babylon::anyflow::GraphBuilder;
using babylon::anyflow::GraphProcessor;

namespace ranker {
namespace ranker{

class RankProcessor : public GraphProcessor {
    //int setup(GraphVertex& vertex) noexcept;
private:
    int process() noexcept override;
    ANYFLOW_INTERFACE (
        ANYFLOW_DEPEND_DATA(RequestInfo, normalized_request)
        ANYFLOW_DEPEND_DATA(PrerankInfo, prerank_info)
        ANYFLOW_EMIT_DATA(RankInfo, rank_info)
    )

};
    
}
}