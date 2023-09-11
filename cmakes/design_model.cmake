#include(${PROJECT_SOURCE_DIR}/third_party/fmt/config.cmake)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/design_model)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/design_model)


aux_source_directory(${PROJECT_SOURCE_DIR}/example/design_model/template_method template_method_dir)
aux_source_directory(${PROJECT_SOURCE_DIR}/example/design_model/strategy  strategy_dir)
aux_source_directory(${PROJECT_SOURCE_DIR}/example/design_model/simple_factory simple_factory_dir)
aux_source_directory(${PROJECT_SOURCE_DIR}/example/design_model/abstract_factory abstract_factory_dir)
aux_source_directory(${PROJECT_SOURCE_DIR}/example/design_model/factory_method factory_method_dir)



message(STATUS "This is SOURCE dir " ${template_method_dir}
                                    ${strategy_dir}
                                    ${simple_factory_dir}
                                    ${abstract_factory_dir})

## 模板方法模式
add_executable(template_method_main ${template_method_dir} example/design_model/template_method_main.cpp)
## 策略模式
add_executable(strategy_main ${strategy_dir} example/design_model/strategy_main.cpp)
## 装饰模式

## 工厂模式
### 简单工厂模式
add_executable(simple_factory_main ${simple_factory_dir} example/design_model/simple_factory_main.cpp)
### 抽象工厂模式
add_executable(abstract_factory_main ${abstract_factory_dir} example/design_model/abstract_factory_main.cpp)
### 工厂方法模式
add_executable(factory_method_main ${factory_method_dir} example/design_model/factory_method_main.cpp)

## 建造者模式
## 原型模式
## 代理模式


