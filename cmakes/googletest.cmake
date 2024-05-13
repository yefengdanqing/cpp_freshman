aux_source_directory(${PROJECT_SOURCE_DIR}/example/googletest gtest_source_dir)

message(STATUS "This is SOURCE dir " ${gtest_source_dir})


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/googletest)

add_executable(googletest_main ${gtest_source_dir})


# target_link_options(googletest_main
#     BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address
# )

add_dependencies(googletest_main ${LIB_DEPENDS})
target_link_libraries(googletest_main ${LIB_BIBRARY})

