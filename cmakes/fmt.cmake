aux_source_directory(${PROJECT_SOURCE_DIR}/example/fmt fmt_source_dir)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/fmt)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/fmt)
add_executable(fmt_main ${fmt_source_dir})
target_link_libraries(fmt_main ${LIB_BIBRARY})

