#抽象成一个函数

aux_source_directory(${PROJECT_SOURCE_DIR}/example/babylon babylon_source_dir)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin/babylon" CACHE FILEPATH "EXE OUTPUT PATH" FORCE)
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/babylon" CACHE FILEPATH "LIBRARY_OUTPUT_PATH" FORCE)





message("skt skt skt skt" ${CMAKE_CXX_FLAGS})
add_executable(graph_start_main example/babylon/graph_start.cpp)
add_dependencies(graph_start_main ${LIB_DEPENDS})
target_link_libraries(graph_start_main ${LIB_BIBRARY})







