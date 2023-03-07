include(${PROJECT_SOURCE_DIR}/third_party/boost/config.cmake)
#抽象成一个函数

aux_source_directory(${PROJECT_SOURCE_DIR}/example/boost source_dir)

message(STATUS "This is SOURCE dir " ${source_dir})


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/boost)
# set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/boost)

add_executable(boost_main ${source_dir})
target_link_libraries(boost_main Boost::thread)

