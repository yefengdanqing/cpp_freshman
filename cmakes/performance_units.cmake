
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




