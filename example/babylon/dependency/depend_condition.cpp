#include "babylon/anyflow/builder.h"
#include "babylon/anyflow/graph.h"

#include "gtest/gtest.h"

using ::babylon::anyflow::Closure;
using ::babylon::anyflow::Graph;
using ::babylon::anyflow::GraphBuilder;
using ::babylon::anyflow::GraphData;
using ::babylon::anyflow::GraphProcessor;

struct ConstProcessor : public GraphProcessor {
  virtual int process() noexcept override {
    *x.emit() = *a;
    return 0;
  }

  // ANYFLOW_INTERFACE(ANYFLOW_DEPEND_DATA(::std::string, a, 1)
  //                       ANYFLOW_EMIT_DATA(::std::string, x))
  ANYFLOW_INTERFACE(ANYFLOW_DEPEND_DATA(::std::string, a)
                        ANYFLOW_EMIT_DATA(::std::string, x))
};

struct MutableProcessor : public GraphProcessor {
  virtual int process() noexcept override {
    *x.emit() = *a;
    return 0;
  }

  ANYFLOW_INTERFACE(ANYFLOW_DEPEND_MUTABLE_DATA(::std::string, a)
                        ANYFLOW_EMIT_DATA(::std::string, x))
};

struct DependencyTest : public ::testing::Test {
  virtual void SetUp() override {
    {
      auto& vertex = builder.add_vertex([] {
        return ::std::unique_ptr<ConstProcessor>(new ConstProcessor);
      });
      vertex.named_depend("a").to("A").on("C1");
      vertex.named_emit("x").to("X");
      vertex.set_name("ConstProcessor1");
    }
    {
      auto& vertex = builder.add_vertex([] {
        return ::std::unique_ptr<ConstProcessor>(new ConstProcessor);
      });
      vertex.named_depend("a").to("A").on("C2");
      vertex.named_emit("x").to("Y");
      vertex.set_name("ConstProcessor2");
    }
    {
      auto& vertex = builder.add_vertex([] {
        return ::std::unique_ptr<MutableProcessor>(new MutableProcessor);
      });
      vertex.named_depend("a").to("A").on("C3");
      vertex.named_emit("x").to("Z");
      vertex.set_name("MutableProcessor");
    }

    builder.finish();
    graph = builder.build();
    a = graph->find_data("A");
    x = graph->find_data("X");
    y = graph->find_data("Y");
    z = graph->find_data("Z");


    c1 = graph->find_data("C1");
    c2 = graph->find_data("C2");
    c3 = graph->find_data("C3");
  }
  GraphBuilder builder;
  ::std::unique_ptr<Graph> graph;

  GraphData* a;
  GraphData* x;
  GraphData* y;
  GraphData* z;
  GraphData* c1;
  GraphData* c2;
  GraphData* c3;
};
TEST_F(DependencyTest,
       immediately_ready_when_target_ready_and_condition_established_1) {
  *c1->emit<bool>() = true;
  *a->emit<std::string>() = "10086";
  auto closure = graph->run(x);
  ASSERT_EQ(0, closure.get());
  ASSERT_EQ("10086", *x->value<::std::string>());
}

TEST_F(DependencyTest,
       immediately_ready_when_target_ready_and_condition_not_established_1) {
  *c1->emit<bool>() = false;
  *a->emit<std::string>() = "10086";
  auto closure = graph->run(x);
  ASSERT_EQ(0, closure.get());
  ASSERT_TRUE(x->empty());
}

TEST_F(DependencyTest, single_mutable_is_ok) {
  *c2->emit<bool>() = false;
  *c3->emit<bool>() = true;
  *a->emit<std::string>() = "10086";
  auto closure = graph->run(z, y);
  ASSERT_EQ(0, closure.get());
  ASSERT_TRUE(y->empty());
  ASSERT_FALSE(z->empty());
  ASSERT_EQ("10086", *z->value<::std::string>());
}