#include "babylon/application_context.h"

#include "gtest/gtest.h"

struct ApplicationContextTest : public ::testing::Test {
  using ApplicationContext = ::babylon::ApplicationContext;
  template <typename T, typename... BS>
  using DefaultComponentHolder =
      ApplicationContext::DefaultComponentHolder<T, BS...>;
  template <typename T, typename... BS>
  using FactoryComponentHolder =
      ApplicationContext::FactoryComponentHolder<T, BS...>;
  using Any = ::babylon::Any;

  static size_t construct_times;
  static size_t initialize_times;

  virtual void SetUp() override {
    construct_times = 0;
    initialize_times = 0;
  }

  ::babylon::ApplicationContext context;
};
size_t ApplicationContextTest::construct_times;
size_t ApplicationContextTest::initialize_times;

TEST_F(ApplicationContextTest, get_component_after_register) {
  ASSERT_FALSE(context.component_accessor<::std::string>());
  context.register_component(DefaultComponentHolder<::std::string>::create());
  ASSERT_TRUE(context.component_accessor<::std::string>());
  for (auto& holder : context) {
    ASSERT_EQ(&::babylon::TypeId<::std::string>::ID, holder.type_id());
    ASSERT_EQ(1, holder.accessible_path_number());
  }
}