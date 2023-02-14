cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER "/usr/local/bin/gcc-12")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-12")
message(STATUS "${CMAKE_C_COMPILER} ${CMAKE_CXX_COMPILER}")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-w -g -pthread")
set(CMAKE_COMPILER_IS_GNUCXX ON)
if(CMAKE_COMPILER_IS_GNUCXX)
    #add_compile_options(20)
    message(STATUS "optional:-std=c++2a")
endif()
# message("sys path:$ENV{PATH}")
# message("find path:${CMAKE_MODULE_PATH}")

#log
set(CMAKE_VERBOSE_MAKEFILE ON)


project(practice_p1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)

message(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
#MESSAGE(STATUS "This is SOURCE dir "${practice_p1_SOURCE_DIR})

message("bin:${EXECUTABLE_OUTPUT_PATH} lib:${LIBRARY_OUTPUT_PATH}--${LIBRARY_OUTPUT_DIRECTORY}")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
# set(LIBRARY_OUTPUT_DIRECTORY)
message("bin:${EXECUTABLE_OUTPUT_PATH} lib:${LIBRARY_OUTPUT_PATH}--${LIBRARY_OUTPUT_DIRECTORY}")

aux_source_directory(${PROJECT_SOURCE_DIR}/src source_dir)
message(STATUS "This is SOURCE dir " ${source_dir})
include_directories(${PROJECT_SOURCE_DIR}/include)


add_executable(${PROJECT_NAME} ${source_dir})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "p1")
target_link_libraries(${PROJECT_NAME} Snappy)

find_package(Snappy REQUIRED)
message(STATUS "Snappy_DIR = ${Snappy_DIR}")
    message(STATUS "Snappy_INCLUDE_DIRS = ${SNAPPY_INCLUDE_DIR}")
    message(STATUS "Snappy_LIBS = ${SNAPPY_LIBRARY}")
if (SNAPPY_FOUND)
    message(STATUS "Snappy_DIR = ${Snappy_DIR}")
    message(STATUS "Snappy_INCLUDE_DIRS = ${SNAPPY_INCLUDE_DIR}")
    message(STATUS "Snappy_LIBS = ${SNAPPY_LIBRARY}")
    include_directories(${SNAPPY_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} Snappy)
else()
    message(FATAL_ERROR "not find snappy")
endif()







message("all path:[${CMAKE_CXX_STANDARD}]--[${CMAKE_CXX_STANDARD_REQUIRED}]--[${CMAKE_CXX_FLAGS}]--[${CMAKE_CXX_FLAGS_RELEASE}]--[${CMAKE_CXX_FLAGS_DEBUG}]")
message("c++ para: [${PROJECT_NAME}]--[${CMAKE_PROJECT_NAME}]--[${PROJECT_SOURCE_DIR}]--[${PROJECT_BINARY_DIR}]--[${name_SOURCE_DIR}]--[${name_BINARY_DIR}]")






