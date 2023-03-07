include(${PROJECT_SOURCE_DIR}/third_party/glog/config.cmake)


aux_source_directory(${PROJECT_SOURCE_DIR}/example/glog source_dir)

message(STATUS "This is SOURCE dir " ${source_dir})


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/glog)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/glog)

add_executable(glog_main ${source_dir})

target_link_libraries(glog_main glog)

