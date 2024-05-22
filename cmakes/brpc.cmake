#抽象成一个函数

aux_source_directory(${PROJECT_SOURCE_DIR}/example/brpc source_dir)
aux_source_directory(${PROJECT_SOURCE_DIR}/example/brpc/common common_dir)

message(STATUS "This is SOURCE dir " ${source_dir})

file(GLOB PBUF_FILES "${PROJECT_SOURCE_DIR}/proto/*.proto")
message(STATUS "proto is PBUF_FILES " ${PBUF_FILES})
custom_protobuf_generate_cpp(PBUF_SRCS PBUF_HDRS ${PBUF_FILES})

message(STATUS "This is PBUF_FILES " ${PBUF_SRCS} ${PBUF_HDRS})

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin/brpc" CACHE FILEPATH "EXE OUTPUT PATH" FORCE)
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/brpc" CACHE FILEPATH "EXE OUTPUT PATH" FORCE)


add_custom_target(
    compile_all_protos_brpc
    DEPENDS ${PBUF_SRCS} ${PBUF_HDRS}
)



add_executable(brpc_client_main example/brpc/brpc_client.cpp ${PBUF_SRCS} ${PBUF_HDRS})
add_dependencies(brpc_client_main compile_all_protos_brpc ${LIB_DEPENDS})
target_link_libraries(brpc_client_main ${LIB_BIBRARY} dl z)


add_executable(brpc_server_main example/brpc/brpc_server.cpp ${PBUF_SRCS} ${PBUF_HDRS} ${common_dir})
add_dependencies(brpc_server_main compile_all_protos_brpc ${LIB_DEPENDS})
target_link_libraries(brpc_server_main dl z ${LIB_BIBRARY})

# add_executable(cassandra_test_main example/brpc/cassandra_test.cpp ${PBUF_SRCS} ${PBUF_HDRS})
# add_dependencies(cassandra_test_main compile_all_protos_brpc)
# target_link_libraries(cassandra_test_main ${LIB_BIBRARY})

# add_executable(brpc_test_main example/brpc/main.cpp ${PBUF_SRCS} ${PBUF_HDRS})
# message("xxxxxx LIB_DEPENDS:${LIB_DEPENDS}")
# add_dependencies(brpc_test_main ${LIB_DEPENDS})
# target_link_libraries(brpc_test_main ${LIB_BIBRARY} dl)




