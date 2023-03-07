include(${PROJECT_SOURCE_DIR}/third_party/googletest/config.cmake)

aux_source_directory(${PROJECT_SOURCE_DIR}/example/googletest source_dir)

message(STATUS "This is SOURCE dir " ${source_dir})


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/googletest)

add_executable(googletest_main ${source_dir})

target_link_libraries(googletest_main gtest_main)

