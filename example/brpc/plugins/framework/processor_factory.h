#pragma once

#include <string>

#include "babylon/anyflow/graph.h"

#include "processor/parse_request.h"
#include "processor/prerank.h"
#include "processor/ranker.h"
#include "processor/output.h"


using babylon::anyflow::GraphProcessor;

namespace utopian {
namespace ranker {
class ProcessorFactory {
public:
    static std::unique_ptr<GraphProcessor> create_processor(const std::string& processor_name) {
        if (processor_name == "ParseRequestProcessor") {
            return std::make_unique<ParseRequestProcessor>();
        } else if (processor_name == "RankProcessor") {
            return std::make_unique<RankProcessor>();
        } else if (processor_name == "OutputProcessor") {
            return std::make_unique<OutputProcessor>();
        } else if (processor_name == "PrerankProcessor") {
            return std::make_unique<PrerankProcessor>();
        } else {
            return nullptr;
        }
    }
};



}
}