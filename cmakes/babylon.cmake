#抽象成一个函数

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/babylon")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib/babylon")
#CMAKE_LIBRARY_OUTPUT_DIRECTORY 或 CMAKE_ARCHIVE_OUTPUT_DIRECTORY 
# first graph
add_executable(graph_start_main ${PROJECT_SOURCE_DIR}/example/babylon/graph_start.cpp ${PROJECT_SOURCE_DIR}/example/babylon/bthread_graph_executor.cpp)
include_directories(${PROJECT_SOURCE_DIR}/example/babylon)
add_dependencies(graph_start_main ${LIB_DEPENDS})
target_link_libraries(graph_start_main ${LIB_BIBRARY} dl)


# graph condition
add_executable(graph_condition ${PROJECT_SOURCE_DIR}/example/babylon/graph_condition.cpp ${PROJECT_SOURCE_DIR}/example/babylon/bthread_graph_executor.cpp)
include_directories(${PROJECT_SOURCE_DIR}/example/babylon)
add_dependencies(graph_condition ${LIB_DEPENDS})
target_link_libraries(graph_condition ${LIB_BIBRARY} dl)
# set_target_properties(graph_condition PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/babylon)
message(STATUS "Source files: ${PROJECT_SOURCE_DIR}/example/babylon/graph_condition.cpp")
message(STATUS "Source files: ${PROJECT_SOURCE_DIR}/example/babylon/bthread_graph_executor.cpp")
message("${LIB_DEPENDS} ${LIB_BIBRARY}")


#babylon_test_main main
# 使用file(GLOB)收集component_application目录下的源文件
file(GLOB_RECURSE application_src ${PROJECT_SOURCE_DIR}/example/babylon/component_application/*.cpp)
# 使用file(GLOB)收集component_queue目录下的源文件
file(GLOB_RECURSE queue_src ${PROJECT_SOURCE_DIR}/example/babylon/component_queue/*.cpp)
# 使用file(GLOB)收集expressor目录下的源文件
file(GLOB_RECURSE expressor_src ${PROJECT_SOURCE_DIR}/example/babylon/expressor/*.cpp)
# 使用file(GLOB)收集dependency目录下的源文件
file(GLOB_RECURSE dependency_src ${PROJECT_SOURCE_DIR}/example/babylon/dependency/*.cpp)

# 设置包含目录
include_directories(
    ${PROJECT_SOURCE_DIR}/example/babylon/component_application
    ${PROJECT_SOURCE_DIR}/example/babylon/component_queue
    ${PROJECT_SOURCE_DIR}/example/babylon/expressor
    ${PROJECT_SOURCE_DIR}/example/babylon/dependency
)

add_executable(babylon_test_main
        ${PROJECT_SOURCE_DIR}/example/babylon/main.cpp
        ${application_src}
        ${queue_src}
        ${expressor_src}
        ${dependency_src})

message("babylon_test_main:${dependency_dir}")
add_dependencies(babylon_test_main ${LIB_DEPENDS})
target_link_libraries(babylon_test_main ${LIB_BIBRARY} dl)
# set_target_properties(babylon_test_main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/babylon)







