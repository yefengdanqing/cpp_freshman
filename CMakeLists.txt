cmake_minimum_required(VERSION 3.17)

message(STATUS "${CMAKE_C_COMPILER} ${CMAKE_CXX_COMPILER}")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-w -g -pthread")
set(CMAKE_COMPILER_IS_GNUCXX ON)

project(cpp_freshman LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

aux_source_directory(${PROJECT_SOURCE_DIR}/example source_dir)

message(STATUS "This is SOURCE dir " ${source_dir})

#传参控制
message("start build ${EXEC_NAME}")
if (EXEC_NAME STREQUAL fmt)
    include(${PROJECT_SOURCE_DIR}/cmakes/fmt.cmake)
elseif(EXEC_NAME STREQUAL spdlog)
    include(${PROJECT_SOURCE_DIR}/cmakes/spdlog.cmake)
elseif(EXEC_NAME STREQUAL boost)
    include(${PROJECT_SOURCE_DIR}/cmakes/boost.cmake)
elseif(EXEC_NAME STREQUAL protobuf)
    include(${PROJECT_SOURCE_DIR}/cmakes/protobuf.cmake)
elseif(EXEC_NAME STREQUAL googletest)
    include(${PROJECT_SOURCE_DIR}/cmakes/googletest.cmake)
elseif(EXEC_NAME STREQUAL empty)
    message("build empty finish")
endif()
message("finish build ${EXEC_NAME}")





