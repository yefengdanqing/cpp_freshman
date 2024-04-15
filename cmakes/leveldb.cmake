#include(${PROJECT_SOURCE_DIR}/third_party/leveldb/config.cmake)


aux_source_directory(${PROJECT_SOURCE_DIR}/example/leveldb source_dir)

message(STATUS "This is SOURCE dir " ${source_dir})


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/leveldb)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/leveldb)

add_executable(leveldb_main ${source_dir})
add_dependencies(leveldb_main ${LIB_DEPENDS})
target_link_libraries(leveldb_main ${LIB_BIBRARY})

