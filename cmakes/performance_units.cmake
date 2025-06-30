
# set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/performance_units" CACHE FILEPATH "EXE OUTPUT PATH" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/performance_units)


file(GLOB_RECURSE struct_pb_dir ${PROJECT_SOURCE_DIR}/example/performance_units/struct_pb_dir/*.cpp)
message(STATUS "Found  struct_pb_dir files: ${struct_pb_dir}")




include_directories(${PROJECT_BINARY_DIR}/protocol/protobuf/cpp)
## struct_pb
add_executable(struct_pb_main
            ${PBUF_SRCS}
            ${PROJECT_SOURCE_DIR}/example/performance_units/struct_pb/example.cpp)
add_dependencies(struct_pb_main  compile_all_protos_brpc ${LIB_DEPENDS})
target_link_libraries(struct_pb_main ${LIB_BIBRARY} dl z stdc++fs)
set_target_properties(struct_pb_main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/performance_units)

## debug cache line
add_executable(debug_cache_line ${PROJECT_SOURCE_DIR}/example/performance_units/debug_cache_line.cpp)
add_dependencies(debug_cache_line ${LIB_DEPENDS})
target_link_libraries(debug_cache_line ${LIB_BIBRARY} dl z stdc++fs)
set_target_properties(debug_cache_line PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/performance_units)

## alignas line
add_executable(virtual_alignas_test ${PROJECT_SOURCE_DIR}/example/performance_units/virtual_alignas_test.cpp)
add_dependencies(virtual_alignas_test ${LIB_DEPENDS})
target_link_libraries(virtual_alignas_test ${LIB_BIBRARY} dl z stdc++fs)
set_target_properties(virtual_alignas_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/performan
ce_units)

## alignas line
add_executable(lock_free_queue ${PROJECT_SOURCE_DIR}/example/performance_units/lock_free_queue.cpp)
add_dependencies(lock_free_queue ${LIB_DEPENDS})
target_link_libraries(lock_free_queue ${LIB_BIBRARY} dl z stdc++fs)
set_target_properties(lock_free_queue PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/performance_units)

#shared_ptr;
add_executable(my_shared_ptr ${PROJECT_SOURCE_DIR}/example/performance_units/my_shared_ptr.cpp)
add_dependencies(my_shared_ptr ${LIB_DEPENDS})
target_link_libraries(my_shared_ptr ${LIB_BIBRARY} dl z stdc++fs)
set_target_properties(my_shared_ptr PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/performance_units)