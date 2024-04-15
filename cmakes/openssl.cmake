include(${PROJECT_SOURCE_DIR}/third_party/openssl/config.cmake)


aux_source_directory(${PROJECT_SOURCE_DIR}/example/openssl source_dir)

message(STATUS "This is SOURCE dir " ${source_dir})


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/openssl)

add_executable(openssl_main ${source_dir})

add_dependencies(openssl_main ${LIB_DEPENDS})
target_link_libraries(openssl_main ${LIB_BIBRARY})

