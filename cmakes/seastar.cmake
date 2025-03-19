include(${PROJECT_SOURCE_DIR}/third_party/seastar/config.cmake)


aux_source_directory(${PROJECT_SOURCE_DIR}/example/seastar source_dir)

message(STATUS "This is SOURCE dir " ${source_dir})


# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/seastar CACHE FILEPATH "EXE OUTPUT PATH" FORCE)
# set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/seastar CACHE FILEPATH "EXE OUTPUT PATH" FORCE)

add_executable(seastar_main ${source_dir})

target_link_libraries(seastar_main seastar)
add_dependencies(openssl_main ${LIB_DEPENDS})
target_link_libraries(openssl_main ${LIB_BIBRARY})

