#抽象成一个函数

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/babylon")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib/babylon")
#CMAKE_LIBRARY_OUTPUT_DIRECTORY 或 CMAKE_ARCHIVE_OUTPUT_DIRECTORY 


#babylon_test_main main
# 使用file(GLOB)收集component_application目录下的源文件
file(GLOB_RECURSE application_src ${PROJECT_SOURCE_DIR}/example/babylon/component_application/*.cpp)
# 使用file(GLOB)收集component_queue目录下的源文件
file(GLOB_RECURSE queue_src ${PROJECT_SOURCE_DIR}/example/babylon/component_queue/*.cpp)
# 使用file(GLOB)收集expressor目录下的源文件
file(GLOB_RECURSE expressor_src ${PROJECT_SOURCE_DIR}/example/babylon/expressor/*.cpp)
# 使用file(GLOB)收集dependency目录下的源文件
file(GLOB_RECURSE dependency_src ${PROJECT_SOURCE_DIR}/example/babylon/dependency/*.cpp)

file(GLOB_RECURSE concurrent_src ${PROJECT_SOURCE_DIR}/example/babylon/concurrent/*.cpp)
# 设置包含目录
include_directories(
    ${PROJECT_SOURCE_DIR}/example/babylon
    ${PROJECT_SOURCE_DIR}/example/babylon/component_application
    ${PROJECT_SOURCE_DIR}/example/babylon/component_queue
    ${PROJECT_SOURCE_DIR}/example/babylon/expressor
    ${PROJECT_SOURCE_DIR}/example/babylon/dependency
    ${PROJECT_SOURCE_DIR}/example/babylon/concurrent
)
message(STATUS "LIB_DEPENDS=${LIB_DEPENDS}")
message(STATUS "LIB_BIBRARY=${LIB_BIBRARY}")


# first graph
add_executable(graph_start_main ${PROJECT_SOURCE_DIR}/example/babylon/graph_start.cpp ${PROJECT_SOURCE_DIR}/example/babylon/bthread_graph_executor.cpp)
add_dependencies(graph_start_main ${LIB_DEPENDS})
target_link_libraries(graph_start_main ${LIB_BIBRARY} dl)


# graph condition
add_executable(graph_condition ${PROJECT_SOURCE_DIR}/example/babylon/graph_condition.cpp ${PROJECT_SOURCE_DIR}/example/babylon/bthread_graph_executor.cpp)
add_dependencies(graph_condition ${LIB_DEPENDS})
target_link_libraries(graph_condition ${LIB_BIBRARY} dl)

#depsit_box
add_executable(concurrent_deposit_main ${PROJECT_SOURCE_DIR}/example/babylon/concurrent/deposit_box_main.cpp)
add_dependencies(concurrent_deposit_main ${LIB_DEPENDS})
target_link_libraries(concurrent_deposit_main ${LIB_BIBRARY} dl)

#concurrent_bound_queue_main
add_executable(concurrent_bound_queue_main ${PROJECT_SOURCE_DIR}/example/babylon/concurrent/bounded_queue_main.cpp)
add_dependencies(concurrent_bound_queue_main ${LIB_DEPENDS})
target_link_libraries(concurrent_bound_queue_main ${LIB_BIBRARY} dl)
# set_target_properties(concurrent_bound_queue_main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/babylon)


add_executable(memory_reusable_test_main ${PROJECT_SOURCE_DIR}/example/babylon/reusable/memory_reusable_test_main.cpp)
add_dependencies(memory_reusable_test_main ${LIB_DEPENDS})
target_link_libraries(memory_reusable_test_main ${LIB_BIBRARY} dl)



add_executable(cmake_test_main2 ${PROJECT_SOURCE_DIR}/example/babylon/concurrent/cmake_test_main.cpp)
add_dependencies(cmake_test_main2 ${LIB_DEPENDS})
target_link_libraries(cmake_test_main2 ${LIB_BIBRARY} dl)


add_executable(babylon_test_main
        ${PROJECT_SOURCE_DIR}/example/babylon/main.cpp
        ${application_src}
        ${queue_src}
        ${expressor_src}
        ${dependency_src})

message("babylon_test_main:${dependency_dir}")
add_dependencies(babylon_test_main ${LIB_DEPENDS})
target_link_libraries(babylon_test_main ${LIB_BIBRARY} dl)







