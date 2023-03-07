include(${PROJECT_SOURCE_DIR}/third_party/brpc/config.cmake)
#抽象成一个函数

aux_source_directory(${PROJECT_SOURCE_DIR}/example/brpc source_dir)

message(STATUS "This is SOURCE dir " ${source_dir})

file(GLOB PBUF_FILES "${PROJECT_SOURCE_DIR}/proto/*.proto")
custom_protobuf_generate_cpp(PBUF_SRCS PBUF_HDRS ${PBUF_FILES})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/brpc)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/brpc)


add_custom_target(
    compile_all_protos
    DEPENDS ${PBUF_SRCS} ${PBUF_HDRS}
)


add_executable(brpc_main ${source_dir})
target_link_libraries(brpc_main brpc protobuf protobuf-lite protoc gflags)
add_dependencies(brpc_main compile_all_protos)

