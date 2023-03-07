cmake_minimum_required(VERSION 3.17)

message(STATUS "${CMAKE_C_COMPILER} ${CMAKE_CXX_COMPILER}")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-w -g -pthread")
set(CMAKE_COMPILER_IS_GNUCXX ON)


project(cpp_freshman LANGUAGES CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmakes")


set(CMAKE_VERBOSE_MAKEFILE ON)

aux_source_directory(${PROJECT_SOURCE_DIR}/example source_dir)
message(STATUS "This is SOURCE dir " ${source_dir})



option(WITH_PROFILER "Compile PaddlePaddle with GPU profiler and gperftools"
       OFF)


find_package (OpenSSL)
if(OpenSSL_FOUND)
    set(OPENSSL_DEPENDS "")
    message("openssl found, libraries = ${OPENSSL_LIBRARIES}, path=${OPENSSL_INCLUDE_DIR}")
    set(OPENSSL_INSTALL_DIR "${OPENSSL_ROOT_DIR}")
else() 
    #include(${PROJECT_SOURCE_DIR}/third_party/openssl/openssl.cmake)
    # set(OPENSSL_DEPENDS openssl_openssl)
    # set(OPENSSL_LIBRARIES "${SSL_LIBRARIES}" "${CRYPTO_LIBRARIES}")
    message("an openssl defined @ Dependency")
endif()
       
if(WITH_PROFILER)
    find_package(Gperftools REQUIRED)
    include_directories(${GPERFTOOLS_INCLUDE_DIR})
    add_definitions(-DWITH_GPERFTOOLS)
endif()



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
elseif(EXEC_NAME STREQUAL brpc)
    include(${PROJECT_SOURCE_DIR}/cmakes/brpc.cmake)
elseif(EXEC_NAME STREQUAL gflags)
    include(${PROJECT_SOURCE_DIR}/cmakes/gflags.cmake)
elseif(EXEC_NAME STREQUAL leveldb)
    include(${PROJECT_SOURCE_DIR}/cmakes/leveldb.cmake)
elseif(EXEC_NAME STREQUAL glog)
    include(${PROJECT_SOURCE_DIR}/cmakes/glog.cmake)
elseif(EXEC_NAME STREQUAL empty)
    message("build empty finish")
endif()
message("finish build ${EXEC_NAME}")















# if(NOT DEFINED PADDLE_LIB)
#   message(FATAL_ERROR "please set PADDLE_LIB with -DPADDLE_LIB=/path/paddle/lib")
# endif()
# if(NOT DEFINED DEMO_NAME)
#   message(FATAL_ERROR "please set DEMO_NAME with -DDEMO_NAME=demo_name")
# endif()