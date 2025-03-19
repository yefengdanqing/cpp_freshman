#抽象成一个函数

# aux_source_directory(${PROJECT_SOURCE_DIR}/example/boost source_dir)

# message(STATUS "This is SOURCE dir " ${source_dir})


# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE FILEPATH "EXE OUTPUT PATH" FORCE)
# # set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib CACHE FILEPATH "EXE OUTPUT PATH" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/boost)

# add_executable(boost_main ${source_dir})
# add_dependencies(boost_main GOOGLETEST)
# target_link_libraries(boost_main Boost::thread Boost::graph gtest_main gtest)
aux_source_directory(${PROJECT_SOURCE_DIR}/example/boost source_dir)
add_executable(boost_main ${source_dir})
add_dependencies(boost_main ${LIB_DEPENDS})
target_link_libraries(boost_main gtest_main gtest)
#target_link_libraries(boost_main ${LIB_BIBRARY})

