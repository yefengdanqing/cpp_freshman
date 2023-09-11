include(${PROJECT_SOURCE_DIR}/third_party/cassandra_driver/config.cmake)


aux_source_directory(${PROJECT_SOURCE_DIR}/example/cassandra_driver source_dir)

message(STATUS "This is SOURCE dir " ${source_dir})


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/cassandra_driver)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/cassandra_driver)

add_executable(cassandra_driver_main ${source_dir})

target_link_libraries(cassandra_driver_main cassandra_driver uv)

