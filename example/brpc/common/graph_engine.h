#include "babylon/anyflow/builder.h"
#include "babylon/anyflow/graph.h"
#include "babylon/concurrent/object_pool.h"


using babylon::anyflow::Graph;
using babylon::anyflow::GraphData;
using babylon::anyflow::Closure;
using babylon::anyflow::GraphBuilder;
using babylon::anyflow::GraphProcessor;
using babylon::anyflow::GraphExecutor;

typedef babylon::ObjectPool<Graph> GraphPool;

class GraphEngine {
public:
    GraphEngine() = default;
    GraphEngine(const GraphEngine&) = delete;

    Graph* get(const std::string& name) noexcept;
    Graph try_get(const std::string& name) noexcept;
    std::unique_ptr<Graph> create() noexcept;
    static void executor(GraphExecutor* executor);
    //   test_gr/baidu/haokan-rec/grc/conf/gflags.conf
    bool init_pool(int size, int cache_size, const std::string& name) noexcept;
private:
    GraphBuilder builder;
    std::unique_ptr<GraphPool> _pool;

    static ::std::unique_ptr<GraphExecutor> _executor;
    std::unordered_map<std::string, std::unique_ptr<GraphPool>> _graphs;
    std::set<std::string> _graph_names;

};



