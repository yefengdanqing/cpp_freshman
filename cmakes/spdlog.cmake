#include(${PROJECT_SOURCE_DIR}/third_party/spdlog/config.cmake)


aux_source_directory(${PROJECT_SOURCE_DIR}/example/spdlog source_dir)

message(STATUS "This is SOURCE dir " ${source_dir})


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/spdlog)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/spdlog)

add_executable(spdlog_main ${source_dir})
add_dependencies(spdlog_main ${LIB_DEPENDS})
target_link_libraries(spdlog_main ${LIB_BIBRARY})


