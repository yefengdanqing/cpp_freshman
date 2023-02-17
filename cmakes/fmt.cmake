include(${PROJECT_SOURCE_DIR}/third_party/fmt/config.cmake)


aux_source_directory(${PROJECT_SOURCE_DIR}/example/fmt source_dir)

message(STATUS "This is SOURCE dir " ${source_dir})


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/fmt)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/fmt)

add_executable(fmt_main ${source_dir})
target_link_libraries(fmt_main fmt)

