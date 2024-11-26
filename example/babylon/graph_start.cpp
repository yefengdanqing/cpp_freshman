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
        std::cout << "x:" << (*x) << std::endl;
        *z.emit() = *x + *y;
        return 0;
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

class MultiProcessor : public GraphProcessor {

private:
    int process() noexcept override {
        std::cout << "z:" << *z << " ,a:" << (*a) << std::endl;
        *b.emit() = (*z) * (*a);
        return 0;
    }
    ANYFLOW_INTERFACE (
        ANYFLOW_DEPEND_DATA(int, z)
        ANYFLOW_DEPEND_DATA(int, a)
        ANYFLOW_EMIT_DATA(int, b)
    )

};

class AllProcessor : public GraphProcessor {

private:
    int process() noexcept override {
        std::cout << "x:" << *x << " ,a:" << (*a) << " ,bb:" << (*b) << std::endl;
        *c.emit() = (*x) + (*a) + (*b);
        return 0;
    }
    ANYFLOW_INTERFACE (
        ANYFLOW_DEPEND_DATA(int, x)
        ANYFLOW_DEPEND_DATA(int, a)
        ANYFLOW_DEPEND_DATA(int, b)
        ANYFLOW_EMIT_DATA(int, c)
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
        v.set_name("PlusProcessor");
    }
    {
        auto& v = builder.add_vertex([]{
            return std::make_unique<MultiProcessor>();
        });
        v.named_depend("z").to("C");
        v.named_depend("a").to("E");
        v.named_emit("b").to("F");
        v.set_name("MultiProcessor");
    }
    {
        auto& v = builder.add_vertex([]{
            return std::make_unique<AllProcessor>();
        });
        v.named_depend("x").to("A");
        v.named_depend("a").to("E");
        v.named_depend("b").to("F");
        v.named_emit("c").to("G");
         v.set_name("AllProcessor");
    }
    builder.finish();
    auto graph = builder.build();

    
    auto* a1 = graph->find_data("A");
    auto* b1 = graph->find_data("B");
    auto* c1 = graph->find_data("C");
    *(a1->emit<int>()) = 100;
    *(b1->emit<int>()) = 200;

    auto* e1 = graph->find_data("E");
    *(e1->emit<int>()) = 2;
    auto* g1 = graph->find_data("G");
    graph->run(g1);
    std::cout << *g1->value<int>() << std::endl;
    return 0;
    

}




