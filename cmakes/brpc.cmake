include(${PROJECT_SOURCE_DIR}/third_party/brpc/config.cmake)
#抽象成一个函数

aux_source_directory(${PROJECT_SOURCE_DIR}/example/brpc source_dir)

message(STATUS "This is SOURCE dir " ${source_dir})

file(GLOB PBUF_FILES "${PROJECT_SOURCE_DIR}/proto/*.proto")
message(STATUS "proto is PBUF_FILES " ${PBUF_FILES})
custom_protobuf_generate_cpp(PBUF_SRCS PBUF_HDRS ${PBUF_FILES})

message(STATUS "This is PBUF_FILES " ${PBUF_SRCS} ${PBUF_HDRS})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/brpc)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/brpc)


add_custom_target(
    compile_all_protos
    DEPENDS ${PBUF_SRCS} ${PBUF_HDRS}
)
set(rpc_libs brpc protobuf protobuf-lite protoc gflags glog dl leveldb ssl crypto z)


add_executable(brpc_client_main example/brpc/brpc_client.cpp ${PBUF_SRCS} ${PBUF_HDRS})
add_dependencies(brpc_client_main compile_all_protos)
target_link_libraries(brpc_client_main ${rpc_libs})


add_executable(brpc_server_main example/brpc/brpc_server.cpp ${PBUF_SRCS} ${PBUF_HDRS})
add_dependencies(brpc_server_main compile_all_protos)
target_link_libraries(brpc_server_main ${rpc_libs})

add_executable(cassandra_test_main example/brpc/cassandra_test.cpp ${PBUF_SRCS} ${PBUF_HDRS})
add_dependencies(cassandra_test_main compile_all_protos)
target_link_libraries(cassandra_test_main ${rpc_libs})



