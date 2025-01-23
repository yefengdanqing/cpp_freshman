#include <iostream>
// #include <promise>
#include <future>

#include "babylon/anyflow/graph.h"
#include "babylon/anyflow/builtin/expression.h"
#include "babylon/logging/interface.h"

#include "bthread_graph_executor.h"

using babylon::anyflow::Graph;
using babylon::anyflow::GraphData;
using babylon::anyflow::Closure;
using babylon::anyflow::GraphBuilder;
using babylon::anyflow::GraphProcessor;
using babylon::anyflow::builtin::ExpressionProcessor;
class PlusProcessor : public GraphProcessor {

private:
    int process() noexcept override {
        std::cout << "x:" << (*x) << ", y:" << (*y) <<std::endl;
        auto committer = z.emit();
        int tmp = *x + *y;
        *committer = tmp;
        std::cout << "z:" << (*committer) << std::endl;
        committer.release();
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
        std::cout << "z:" << *z << ", a:" << (*a) << std::endl;
        auto committer = b.emit();
        *committer = (*z) * (*a);
        std::cout << "b:" << (*committer) << std::endl;
        committer.release();
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
        std::cout << "x:" << *x << ", a:" << (*a) << ", b:" << (*b) << std::endl;
        auto committer = c.emit();
        *committer = (*x) + (*a) + (*b);
        std::cout << "c:" << (*committer) << std::endl;
        committer.release();
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
    builder.set_executor(utopian::framework::BthreadGraphExecutor::instance());
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
    // auto* c1 = graph->find_data("C");
    *(a1->emit<int>()) = 100;
    *(b1->emit<int>()) = 200;

    auto* e1 = graph->find_data("E");
    *(e1->emit<int>()) = 2;
    auto* g1 = graph->find_data("G");
    Closure closure = graph->run(g1);
    std::promise<int> p;
    auto f = p.get_future();
    //要么异步回调等待，要么同步执行
    closure.on_finish([&, graph = std::move(graph)](Closure&&){
        std::cout << "on_finish call ...." << std::endl;
        p.set_value(10);
        std::cout << "last:" << *g1->value<int>() << std::endl;
        graph->reset();
    });
    std::cout << f.get() << std::endl;
    // 只有closure.wait()返回后，graph的执行才彻底结束，可以析构或reset回收以便再次使用
    // closure.wait();
    //~closure;自动调用wait
    // bool finished = closure.finished();
    // auto error_code = closure.get();
    // LOG(INFO) << error_code;

    //core了
    //std::cout << "last:" << *g1->value<int>() << std::endl;

    
}




