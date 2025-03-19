aux_source_directory(${PROJECT_SOURCE_DIR}/example/cassandra_driver source_dir)

message(STATUS "This is SOURCE dir " ${source_dir})


# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/)
# set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_executable(cassandra_driver_main ${source_dir})

# target_link_libraries(cassandra_driver_main scylla-cpp-driver uv)

add_dependencies(cassandra_driver_main ${LIB_DEPENDS})
target_link_libraries(cassandra_driver_main ${LIB_BIBRARY})

