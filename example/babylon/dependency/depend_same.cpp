#include "babylon/anyflow/builder.h"
#include "babylon/anyflow/graph.h"

#include "gtest/gtest.h"

using ::babylon::anyflow::Closure;
using ::babylon::anyflow::Graph;
using ::babylon::anyflow::GraphBuilder;
using ::babylon::anyflow::GraphData;
using ::babylon::anyflow::GraphProcessor;



struct StartProcessor : public GraphProcessor {
  virtual int process() noexcept override {
    *b.emit() = *a;
    *c.emit() = *a;
    *c2.emit() = false;
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
    return 0;
  }
  ANYFLOW_INTERFACE(ANYFLOW_DEPEND_DATA(::std::string, b)
                    ANYFLOW_DEPEND_DATA(::std::string, c)
                        ANYFLOW_EMIT_DATA(::std::string, e)
                        ANYFLOW_EMIT_DATA(::std::string, f))
};
struct EndProcessor : public GraphProcessor {
  virtual int process() noexcept override {
    BABYLON_LOG(WARNING) << "GraphProcessor";
    if (f != nullptr && !f->empty()) {
        BABYLON_LOG(WARNING) << "depend data f is not empty";    
        *g.emit() = *e + *f;
    } else {
        BABYLON_LOG(WARNING) << "depend data f is empty";
        *g.emit() = *e;
    }
    return 0;
  }
  ANYFLOW_INTERFACE(ANYFLOW_DEPEND_DATA(::std::string, e)
                    ANYFLOW_DEPEND_DATA(::std::string, f, 1)
                        ANYFLOW_EMIT_DATA(::std::string, g))
};

struct DependSameTest : public ::testing::Test {
    virtual void SetUp() override {
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
            auto& vertex = builder.add_vertex([] {
                return ::std::unique_ptr<EndProcessor>(new EndProcessor);
            });
            vertex.named_depend("e").to("E");
            vertex.named_depend("f").to("F").on("C2");
            vertex.named_emit("g").to("G");
            vertex.set_name("EndProcessor");
        }
        // {
        //     auto& vertex = builder.add_vertex([] {
        //         return ::std::unique_ptr<EndProcessor>(new EndProcessor);
        //     });
        //     vertex.named_depend("e").to("E");
        //     vertex.named_depend("f").to("B");
        //     vertex.named_emit("g").to("H");
        //     vertex.set_name("EndProcessor");
        // }
        builder.finish();
        graph = builder.build();

        a = graph->find_data("A");
        // x = graph->find_data("X");
        g = graph->find_data("G");
        // h = graph->find_data("H");

        c1 = graph->find_data("C1");
        // c2 = graph->find_data("C2");
    }

    GraphBuilder builder;
    ::std::unique_ptr<Graph> graph;

    GraphData* a;
    GraphData* g;
    GraphData* h;
    // GraphData* y;
    // GraphData* z;
    GraphData* c1;
    // GraphData* c2;
};
TEST_F(DependSameTest, condition_is_all_true) {
    *a->emit<std::string>() = "10086";
    *c1->emit<bool>() = true;
    // *c2->emit<bool>() = true;
    auto closure = graph->run(g);
    ASSERT_EQ(0, closure.get());
    ASSERT_TRUE(g->empty());
    ASSERT_EQ("10086", *g->value<::std::string>());
    // closure = graph->run(h);
    // ASSERT_EQ(0, closure.get());
    // ASSERT_EQ("1008610086", *h->value<::std::string>());
}

// TEST_F(DependSameTest, second_condition_is_all_false) {
//     *a->emit<std::string>() = "10086";
//     *c1->emit<bool>() = true;
//     auto closure = graph->run(y);
//     ASSERT_EQ(0, closure.get());
//     ASSERT_EQ("10000", *y->value<::std::string>());
// }