cmake_minimum_required(VERSION 3.16)

#set(CMAKE_C_COMPILER "/usr/local/bin/gcc-12")
#set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-12")
message(STATUS "${CMAKE_C_COMPILER} ${CMAKE_CXX_COMPILER}")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-w -g -pthread")
set(CMAKE_COMPILER_IS_GNUCXX ON)
if(CMAKE_COMPILER_IS_GNUCXX)
    #add_compile_options(20)
    message(STATUS "optional:-std=c++2a")
endif()
# message("sys path:$ENV{PATH}")
# message("find path:${CMAKE_MODULE_PATH}")

#log
message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")

 message(STATUS "CMAKE_ROOT = ${CMAKE_ROOT}")
set(CMAKE_VERBOSE_MAKEFILE ON)


project(practice_p1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)

message(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
#MESSAGE(STATUS "This is SOURCE dir "${practice_p1_SOURCE_DIR})

message("bin:${EXECUTABLE_OUTPUT_PATH} lib:${LIBRARY_OUTPUT_PATH}--${LIBRARY_OUTPUT_DIRECTORY}")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
# set(LIBRARY_OUTPUT_DIRECTORY)
message("bin:${EXECUTABLE_OUTPUT_PATH} lib:${LIBRARY_OUTPUT_PATH}--${LIBRARY_OUTPUT_DIRECTORY}")

aux_source_directory(${PROJECT_SOURCE_DIR}/src source_dir)
message(STATUS "This is SOURCE dir " ${source_dir})
include_directories(${PROJECT_SOURCE_DIR}/include)


add_executable(${PROJECT_NAME} ${source_dir})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "p1")

find_package(CURL REQUIRED)

if (CURL_FOUND)
    message(STATUS "CURL_DIR = ${CURL_DIR}")
    message(STATUS "CURL_INCLUDE_DIRS = ${CURL_INCLUDE_DIRS}")
    message(STATUS "CURL_LIBS = ${CURL_LIBRARIES}")
    message(STATUS "CURL_VERSION = ${CURL_VERSION_STRING}")
    include_directories(${CURL_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} curl)
else()
    message(FATAL_ERROR "not find curl")
endif()




enable_testing()
find_package(GTest REQUIRED)
if(GTest_FOUND)
    message(STATUS "GTest_DIR = ${GTest_DIR}")
    message(STATUS "GTest_ROOT = ${GTest_ROOT}")
    message(STATUS "GTest_INCLUDE_DIRS = ${GTEST_INCLUDES}")
    message(STATUS "GTest_LIBS = ${GTEST_LIBS}")
else()
    message("fatal")
endif()
target_link_libraries(${PROJECT_NAME} gtest gtest_main)
# add_test(AllTestsInFoo ${PROJECT_NAME})

find_library(test_gtest_path curl)
message("test_gtest_path:${test_gtest_path}")

message("library path ${CMAKE_LIBRARY_PATH}--${CMAKE_SYSTEM_PREFIX_PATH}--${CMAKE_SYSTEM_FRAMEWORK_PATH}--${CMAKE_SYSTEM_APPBUNDLE_PATH}--${CMAKE_FIND_USE_CMAKE_ENVIRONMENT_PATH}")
message("CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH:${CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH}")
message("CMAKE_PREFIX_PATH:${CMAKE_PREFIX_PATH}")
message("CMAKE_FIND_ROOT_PATH:${CMAKE_FIND_ROOT_PATH}")



find_package(spdlog REQUIRED)
if(spdlog_FOUND)
message(STATUS "spdlog_DIR = ${spdlog_DIR}")
message(STATUS "spdlog_INCLUDE_DIRS = ${spdlog_INCLUDES}")
message(STATUS "spdlog_LIBS = ${spdlog_LIBS}")
else()
    message("spdlog not find")
endif()


# find_package(GoogleTest REQUIRED config)
# if (googletest_FOUND)
#     message(STATUS "GTEST_DIR = ${GoogleTest_DIR}")
#     message(STATUS "GTEST_INCLUDE_DIRS = ${GTEST_INCLUDE_DIR}")
#     message(STATUS "GTEST_LIBS = ${GTEST_LIBRARY}")
# else()
# message(FATAL_ERROR "not find gtest")
# endif()




# find_package(SNAPPY REQUIRED)
# message(STATUS "Snappy_DIR = ${SNAPPY_DIR}")
#     message(STATUS "Snappy_INCLUDE_DIRS = ${SNAPPY_INCLUDE_DIR}")
#     message(STATUS "Snappy_LIBS = ${SNAPPY_LIBRARY}")
# if (SNAPPY_FOUND)
#     message(STATUS "Snappy_DIR = ${Snappy_DIR}")
#     message(STATUS "Snappy_INCLUDE_DIRS = ${SNAPPY_INCLUDE_DIR}")
#     message(STATUS "Snappy_LIBS = ${SNAPPY_LIBRARY}")
#     include_directories(${SNAPPY_INCLUDE_DIRS})
#     target_link_libraries(${PROJECT_NAME} Snappy)
# else()
#     message(FATAL_ERROR "not find snappy")
# endif()
# target_link_libraries(${PROJECT_NAME} snappy)







message("all path:[${CMAKE_CXX_STANDARD}]--[${CMAKE_CXX_STANDARD_REQUIRED}]--[${CMAKE_CXX_FLAGS}]--[${CMAKE_CXX_FLAGS_RELEASE}]--[${CMAKE_CXX_FLAGS_DEBUG}]")
message("c++ para: [${PROJECT_NAME}]--[${CMAKE_PROJECT_NAME}]--[${PROJECT_SOURCE_DIR}]--[${PROJECT_BINARY_DIR}]--[${name_SOURCE_DIR}]--[${name_BINARY_DIR}]")






