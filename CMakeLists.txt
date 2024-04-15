cmake_minimum_required(VERSION 3.17)
set(CMAKE_C_COMPILER /usr/local/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/local/bin/g++)

message(STATUS "CMAKE_C_COMPILER:${CMAKE_C_COMPILER} ${CMAKE_CXX_COMPILER}")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_COMPILER_IS_GNUCXX ON)
#默认一般O2的优化
set(CMAKE_CXX_FLAGS "-w -g -pthread -O0 -fno-omit-frame-pointer -Werror=return-type")
set(build_type "")
if (CMAKE_BUILD_TYPE STREQUAL "Asan")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")    
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

project(cpp_freshman LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmakes)

set(LIB_DEPENDS "" CACHE STRING "all")
set(LIB_BIBRARY "" CACHE STRING "all .a or .so file")
set(CMAKE_VERBOSE_MAKEFILE ON)
#######
##增加库依赖
include(${PROJECT_SOURCE_DIR}/external/base.cmake)
#一些依赖库的内容
#######



option(WITH_PROFILER "Compile PaddlePaddle with GPU profiler and gperftools"
       OFF)
# find_package (OpenSSL)
# if(OpenSSL_FOUND)
#     set(OPENSSL_DEPENDS "")
#     message("openssl found, libraries = ${OPENSSL_LIBRARIES}, path=${OPENSSL_INCLUDE_DIR}")
#     set(OPENSSL_INSTALL_DIR "${OPENSSL_ROOT_DIR}")
# else() 
#     include(${PROJECT_SOURCE_DIR}/third_party/openssl/openssl.cmake)
#     set(OPENSSL_DEPENDS openssl_openssl)
#     set(OPENSSL_LIBRARIES "${SSL_LIBRARIES}" "${CRYPTO_LIBRARIES}")
#     message("an openssl defined @ Dependency")
# endif()


if(WITH_PROFILER)
    find_package(Gperftools REQUIRED)
    include_directories(${GPERFTOOLS_INCLUDE_DIR})
    add_definitions(-DWITH_GPERFTOOLS)
endif()

# find_package(Sanitizers)
# if(Sanitizers_FOUND)
#     message("Sanitizers_FOUND")
# endif()



# #传参控制
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
elseif(EXEC_NAME STREQUAL design_model)
    include(${PROJECT_SOURCE_DIR}/cmakes/design_model.cmake)
elseif(EXEC_NAME STREQUAL seastar)
    include(${PROJECT_SOURCE_DIR}/cmakes/seastar.cmake)
elseif(EXEC_NAME STREQUAL openssl)
    include(${PROJECT_SOURCE_DIR}/cmakes/openssl.cmake)
elseif(EXEC_NAME STREQUAL cassandra_driver)
    include(${PROJECT_SOURCE_DIR}/cmakes/cassandra_driver.cmake)
elseif(EXEC_NAME STREQUAL empty)
    include(${PROJECT_SOURCE_DIR}/cmakes/googletest.cmake)
    include(${PROJECT_SOURCE_DIR}/cmakes/brpc.cmake)
    message("build default googltest and brcp finish")
endif()

message("finish build ${EXEC_NAME}")

