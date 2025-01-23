#include "babylon/anyflow/builder.h"
#include "babylon/anyflow/builtin/expression.h"
// #include "bthread_graph_executor.h"
#include "babylon/anyflow/graph.h"
#include "babylon/logging/interface.h"

#include "gtest/gtest.h"

using ::babylon::anyflow::GraphBuilder;
using ::babylon::anyflow::GraphProcessor;
using ::babylon::anyflow::builtin::ExpressionProcessor;

using ::babylon::Any;

class OneProcessor : public GraphProcessor {
    virtual int process() noexcept override {
        auto depend = vertex().anonymous_dependency(0);
        auto index = vertex().index_for_named_dependency("a");
        auto ndepend = index >= 0 ? vertex().named_dependency(index) : nullptr;
        auto emit = vertex().anonymous_emit(0);
        if (depend != nullptr && emit != nullptr) {
            BABYLON_LOG(INFO) << "forward";
            emit->forward(*depend);
        } else if (ndepend != nullptr && emit != nullptr) {
            emit->forward(*ndepend);
        } else {
            BABYLON_LOG(INFO) << "none";
        }
    }
};

class MProcessor : public GraphProcessor {
  virtual int setup() noexcept override {
    auto depend = vertex().anonymous_dependency(0);
    depend->declare_mutable();
    return 0;
  }

  virtual int process() noexcept override {
    auto depend = vertex().anonymous_dependency(0);
    auto emit = vertex().anonymous_emit(0);
    if (depend != nullptr && emit != nullptr) {
      BABYLON_LOG(INFO) << "forward";
      emit->forward(*depend);
    } else {
      BABYLON_LOG(INFO) << "none";
    }
    return 0;
  }
};

struct ExpressionTest : public testing::Test {
  GraphBuilder builder;
  ::std::function<::std::unique_ptr<GraphProcessor>()> processor_creator = [] {
    return ::std::unique_ptr<OneProcessor>(new OneProcessor);
  };
  ::std::function<::std::unique_ptr<GraphProcessor>()> mprocessor_creator = [] {
    return ::std::unique_ptr<MProcessor>(new MProcessor);
  };
};

TEST_F(ExpressionTest, do_correct_caculation) {
  ::std::string exp = "!(A > 3) || B + 1 == C * 3";
  GraphBuilder builder;
  ASSERT_EQ(0, ExpressionProcessor::apply(builder, "D", exp));
  ASSERT_EQ(0, builder.finish());
  auto graph = builder.build();
  ASSERT_TRUE(graph);
  *(graph->find_data("A")->emit<double>()) = 3.5;
  *(graph->find_data("B")->emit<int32_t>()) = 5;
  *(graph->find_data("C")->emit<int32_t>()) = 3;
  ASSERT_EQ(0, graph->run(graph->find_data("D")).get());
  ASSERT_FALSE(graph->find_data("D")->as<bool>());
  graph->reset();
  *(graph->find_data("A")->emit<double>()) = 2.5;
  *(graph->find_data("B")->emit<int32_t>()) = 5;
  *(graph->find_data("C")->emit<int32_t>()) = 3;
  ASSERT_EQ(0, graph->run(graph->find_data("D")).get());
  ASSERT_TRUE(graph->find_data("D")->as<bool>());
  graph->reset();
  *(graph->find_data("A")->emit<double>()) = 3.5;
  *(graph->find_data("B")->emit<int32_t>()) = 5;
  *(graph->find_data("C")->emit<int32_t>()) = 2;
  ASSERT_EQ(0, graph->run(graph->find_data("D")).get());
  ASSERT_TRUE(graph->find_data("D")->as<bool>());
}
