aux_source_directory(${PROJECT_SOURCE_DIR}/example/protobuf protobuf_source_dir)
aux_source_directory(${PROJECT_SOURCE_DIR}/build/protocol/protobuf/cpp protocol_dir)

message(STATUS "This is SOURCE dir " ${protobuf_source_dir})
file(GLOB PBUF_FILES "${PROJECT_SOURCE_DIR}/proto/*.proto")
message("proto buf files: ${PBUF_FILES}")

CUSTOM_PROTOBUF_GENERATE_CPP(PBUF_SRCS PBUF_HDRS ${PBUF_FILES})
message("protobuf result files:[${PBUF_SRCS}][${PBUF_HDRS}]")


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/protobuf)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/protobuf)

add_executable(protobuf_main ${protobuf_source_dir} ${PBUF_SRCS} ${protocol_dir})


# add_custom_command(
#     OUTPUT ${PROJECT_SOURCE_DIR}/build/protocol/protobuf/cpp/person.pb.cc
#     ${PROJECT_SOURCE_DIR}/build/protocol/protobuf/cpp/person.pb.h
#     COMMAND ${PBUF_PROTOC} --cpp_out=${PROJECT_SOURCE_DIR}/build/protocol/protobuf/cpp --proto_path=${PROJECT_SOURCE_DIR}/proto ${PROJECT_SOURCE_DIR}/proto/person.proto
#     DEPENDS ${PROJECT_SOURCE_DIR}/proto/person.proto
#     WORKING_DIRECTORY /home/kaitao.song/cpp_freshman/build/third_party/protobuf/bin/
#     COMMENT "Generate Cpp Protobuf Source Files")




# add_dependencies(protobuf_main compile_all_protos)
# target_link_libraries(protobuf_main protoc protobuf protobuf-lite)
#target_link_libraries(protobuf_main protobuf)
add_custom_target(
    compile_all_protos
    DEPENDS ${PBUF_SRCS} ${PBUF_HDRS}
)
add_dependencies(protobuf_main ${LIB_DEPENDS} compile_all_protos)
target_link_libraries(protobuf_main ${LIB_BIBRARY})

