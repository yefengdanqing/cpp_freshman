#pragma once

#include "babylon/anyflow/builder.h"
#include "babylon/anyflow/graph.h"
#include "demo_proto.pb.h"

#include "common/common.h"


using babylon::anyflow::Graph;
using babylon::anyflow::GraphData;
using babylon::anyflow::Closure;
using babylon::anyflow::GraphBuilder;
using babylon::anyflow::GraphProcessor;



namespace utopian {
namespace ranker{

class ParseRequestProcessor : public GraphProcessor {
private:
    // int setup(GraphVertex& vertex) noexcept;
    int process() noexcept override;
    ANYFLOW_INTERFACE (
        ANYFLOW_DEPEND_DATA(ranker_server::RsRequest, request)
        ANYFLOW_EMIT_DATA(RequestInfo, normalized_request)
    )
};
    
}
}