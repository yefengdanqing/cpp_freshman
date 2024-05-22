#include <iostream>

#include "babylon/anyflow/graph.h"
#include "babylon/logging/interface.h"

using babylon::anyflow::Graph;
using babylon::anyflow::GraphData;
using babylon::anyflow::Closure;
using babylon::anyflow::GraphBuilder;
using babylon::anyflow::GraphProcessor;

class PlusProcessor : public GraphProcessor {

private:
    int process() noexcept override {
        *z.emit() = *x + *y;
    }
    // int* x = nullptr;
    // int* y = nullptr;
    // OutputData<int> result;
    ANYFLOW_INTERFACE (
        ANYFLOW_DEPEND_DATA(int, x)
        ANYFLOW_DEPEND_DATA(int, y)
        ANYFLOW_EMIT_DATA(int, z)
    )

};
int main(int argc, char** argv) {
    GraphBuilder builder;
    builder.set_name("first graph");
    BABYLON_LOG(INFO) << "name:" << builder.name();
    {
        auto& v = builder.add_vertex([]{
            return std::make_unique<PlusProcessor>();
        });
        v.named_depend("x").to("A");
        v.named_depend("y").to("B");
        v.named_emit("z").to("C");
    }
    builder.finish();
    auto graph = builder.build();
    auto* a = graph->find_data("A");
    auto* b = graph->find_data("B");
    auto* c = graph->find_data("C");
    *(a->emit<int>()) = 100;
    *(b->emit<int>()) = 200;

    graph->run(c);
    std::cout << *c->value<int>() << std::endl;
    return 0;
    

}




