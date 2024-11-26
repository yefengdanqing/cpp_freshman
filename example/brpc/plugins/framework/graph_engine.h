#pragma once
#include "babylon/anyflow/builder.h"
#include "babylon/anyflow/graph.h"
#include "babylon/anyflow/executor.h"
#include "babylon/concurrent/object_pool.h"
#include "babylon/application_context.h"


using babylon::anyflow::Graph;
using babylon::anyflow::GraphData;
using babylon::anyflow::Closure;
using babylon::anyflow::GraphBuilder;
using babylon::anyflow::GraphProcessor;
using babylon::anyflow::GraphExecutor;
using babylon::anyflow::ThreadPoolGraphExecutor;
using ::babylon::ApplicationContext;

namespace utopian {
namespace framework {

typedef babylon::ObjectPool<Graph> GraphPool;


struct VertexMessage {
    std::string name;
    std::vector<std::string> emits;
    std::vector<std::string> depends;
    VertexMessage() = default;
    VertexMessage(const VertexMessage &lhs) = default;
    VertexMessage(VertexMessage &&rhs) = default;
};
class GraphEngine {
public:
    GraphEngine() = default;
    GraphEngine(const GraphEngine&) = delete;

    Graph* try_get(const std::string& name) noexcept;
    std::unique_ptr<Graph>& create() noexcept;
    static void executor(GraphExecutor* executor);
    //   test_gr/baidu/haokan-rec/grc/conf/gflags.conf
    bool init_pool(int size, int cache_size, const std::string& name) noexcept;
    int initialize() noexcept;

private:
    void check_executor();
private:
    GraphBuilder builder;
    std::unique_ptr<GraphPool> _pool;
    //静态的执行器用来指定图怎么执行
    static ::std::unique_ptr<GraphExecutor> _executor;

    std::unique_ptr<Graph> _graph;
    std::unordered_map<std::string, std::unique_ptr<GraphPool>> _graphs;
    std::set<std::string> _graph_names;
    std::vector<VertexMessage> _vertexs_message;

};

}
}



