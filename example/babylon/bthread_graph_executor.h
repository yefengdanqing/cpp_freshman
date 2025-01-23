//#pragma once
#include "babylon/anyflow/closure.h"
#include "babylon/anyflow/executor.h"
#include "babylon/anyflow/vertex.h"

#include "bthread/bthread.h"

using babylon::anyflow::ClosureContext;
using babylon::anyflow::GraphData;
using babylon::anyflow::Closure;
using babylon::anyflow::GraphBuilder;
using babylon::anyflow::GraphVertex;
using babylon::anyflow::GraphVertexBuilder;
using babylon::anyflow::GraphVertexClosure;
using babylon::anyflow::GraphExecutor;

namespace utopian {
namespace framework {

class BthreadGraphExecutor : public GraphExecutor {
 public:
  static BthreadGraphExecutor& instance();
  virtual Closure create_closure() noexcept override;
  virtual int run(GraphVertex* vertex,
                  GraphVertexClosure&& closure) noexcept override;
  virtual int run(ClosureContext* closure,
                  Closure::Callback* callback) noexcept override;
};

}
}