include(${PROJECT_SOURCE_DIR}/third_party/gflags/config.cmake)

aux_source_directory(${PROJECT_SOURCE_DIR}/example/gflags source_dir)

message(STATUS "This is SOURCE dir " ${source_dir})


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/gflags)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/gflags)

add_executable(gflags_main ${source_dir})
target_link_libraries(gflags_main gflags pthread)


message("module local: ${gflags_BINARY_DIR}/ ${CMAKE_MODULE_PATH}")
#list(APPEND CMAKE_MODULE_PATH ${gflags_BINARY_DIR})
#list(APPEND CMAKE_PREFIX_PATH ${gflags_BINARY_DIR})
message("${CMAKE_MODULE_PATH}")
