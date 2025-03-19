#include <iostream>
// #include <promise>
#include <future>

#include "babylon/anyflow/graph.h"
#include "babylon/logging/interface.h"
#include "babylon/anyflow/builtin/expression.h"

#include "bthread_graph_executor.h"

using babylon::anyflow::Graph;
using babylon::anyflow::GraphData;
using babylon::anyflow::Closure;
using babylon::anyflow::GraphBuilder;
using babylon::anyflow::GraphProcessor;
using babylon::anyflow::builtin::ExpressionProcessor;


struct StartProcessor : public GraphProcessor {
  virtual int process() noexcept override {
    *b.emit() = *a;
    *c.emit() = *a;
    *c2.emit() = false;
    BABYLON_LOG(WARNING) << "StartProcessor";
    return 0;
  }

    ANYFLOW_INTERFACE(
        ANYFLOW_DEPEND_DATA(::std::string, a)
        ANYFLOW_EMIT_DATA(::std::string, b)
        ANYFLOW_EMIT_DATA(::std::string, c)
        ANYFLOW_EMIT_DATA(bool, c2)
    )
};

struct MiddelProcessor : public GraphProcessor {
  virtual int process() noexcept override {
    *e.emit() = *b;
    *f.emit() = *c;
    BABYLON_LOG(WARNING) << "MiddelProcessor";
    return 0;
  }
  ANYFLOW_INTERFACE(ANYFLOW_DEPEND_DATA(::std::string, b)
                    ANYFLOW_DEPEND_DATA(::std::string, c)
                        ANYFLOW_EMIT_DATA(::std::string, e)
                        ANYFLOW_EMIT_DATA(::std::string, f))
};
struct EndProcessor : public GraphProcessor {
  virtual int process() noexcept override {
    BABYLON_LOG(WARNING) << "EndProcessor";
    // if (f != nullptr && !f->empty()) {
    //     BABYLON_LOG(WARNING) << "depend data f not empty";    
    //     *g.emit() = *e + *f;
    // } else {
    //     BABYLON_LOG(WARNING) << "depend data f xxx empty";
    //     *g.emit() = *e;
    // }
    BABYLON_LOG(WARNING) << "result:" << *e << " + " << *h;
    auto committer = g.emit();
    *committer = *e + *h;
    BABYLON_LOG(WARNING) << "g:" << *committer;
    return 0;
  }
  ANYFLOW_INTERFACE(ANYFLOW_DEPEND_DATA(::std::string, e)
                    ANYFLOW_DEPEND_DATA(::std::string, h)
                        ANYFLOW_EMIT_DATA(::std::string, g))
};

struct EndSameProcessor : public GraphProcessor {
  virtual int process() noexcept override {
    BABYLON_LOG(WARNING) << "GraphProcessor";
    *h.emit() = *e;

    return 0;
  }
  ANYFLOW_INTERFACE(ANYFLOW_DEPEND_DATA(::std::string, e)
                        ANYFLOW_EMIT_DATA(::std::string, h))
};


int main(int argc, char** argv) {
    GraphBuilder builder;
    builder.set_name("first graph");
    // builder.set_executor(utopian::framework::BthreadGraphExecutor::instance());
    BABYLON_LOG(INFO) << "name:" << builder.name();
    
        {
            auto& vertex = builder.add_vertex([] {
                return ::std::unique_ptr<StartProcessor>(new StartProcessor);
            });
            vertex.named_depend("a").to("A").on("C1");
            vertex.named_emit("b").to("B");
            vertex.named_emit("c").to("C");
            vertex.named_emit("c2").to("C2");
            vertex.set_name("StartProcessor");
        }
        {
            auto& vertex = builder.add_vertex([] {
                return ::std::unique_ptr<MiddelProcessor>(new MiddelProcessor);
            });
            vertex.named_depend("b").to("B");
            vertex.named_depend("c").to("C");
            vertex.named_emit("e").to("E");
            vertex.named_emit("f").to("F");
            vertex.set_name("MiddelProcessor");
        }
        {

        }
        {
            auto& vertex = builder.add_vertex([] {
                return ::std::unique_ptr<EndProcessor>(new EndProcessor);
            });
            vertex.named_depend("e").to("E");
            vertex.named_depend("h").to("H"); //结合条件表达式使用
            vertex.named_emit("g").to("G");
            vertex.set_name("EndProcessor");
        }
        // {
        //     auto& vertex = builder.add_vertex([] {
        //         return ::std::unique_ptr<EndSameProcessor>(new EndProcessor);
        //     });
        //     vertex.named_depend("e").to("E");
        //     vertex.named_emit("h").to("H");
        //     vertex.set_name("EndSameProcessor");
        // }

        ExpressionProcessor::apply(builder, "H", "C2 ? E: F");
        builder.finish();
        auto graph = builder.build();

        auto a = graph->find_data("A");
        // x = graph->find_data("X");
        auto g = graph->find_data("G");
        // h = graph->find_data("H");

        auto c1 = graph->find_data("C1");
        // c2 = graph->find_data("C2");
        *a->emit<std::string>() = "10086";
        *c1->emit<bool>() = true;
        // *c2->emit<bool>() = true;
        auto closure = graph->run(g);
        // auto tmp = *g->value<::std::string>();
        BABYLON_LOG(INFO) << "result code:" << closure.get() << " , data:" << *g->value<std::string>();
        // BABYLON_LOG(INFO) << ", result:" << tmp;


    return 0;
}