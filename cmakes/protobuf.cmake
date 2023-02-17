include(${PROJECT_SOURCE_DIR}/third_party/protobuf/config.cmake)

aux_source_directory(${PROJECT_SOURCE_DIR}/example/protobuf source_dir)

message(STATUS "This is SOURCE dir " ${source_dir})


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/protobuf)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/protobuf)

add_executable(protobuf_main ${source_dir})

target_link_libraries(protobuf_main protobuf)