#include(${CMAKE_CURRENT_LIST_DIR}/../libuv/config.cmake)


include(ExternalProject)
# set(EXTERNAL_PREFIX ${CMAKE_BINARY_DIR}/external)
set(CASSANDRA_DRIVER_INSTALL_DIR "${PROJECT_BINARY_DIR}/external/cassandra_driver")
set(CASSANDRA_DRIVER_INCLUDE_DIR "${CASSANDRA_DRIVER_INSTALL_DIR}/include" CACHE PATH "cassandra_driver include directory." FORCE)
set(CASSANDRA_DRIVER_LIBRARIES "${CASSANDRA_DRIVER_INSTALL_DIR}/lib/libscylla-cpp-driver.so" CACHE FILEPATH "CASSANDRA_DRIVER_LIBRARIES" FORCE)

set(CASSANDRA_DRIVER_ROOT ${EXTERNAL_PREFIX}/cassandra_driver)
set(CASSANDRA_DRIVER_GIT_TAG  master)  # 指定版本
set(CASSANDRA_DRIVER_GIT_URL https://github.com/scylladb/cpp-driver.git)  # 指定git仓库地址


set(CASSANDRA_DRIVER_CONFIGURE    cd ${CASSANDRA_DRIVER_ROOT}/src/CASSANDRA_DRIVER && rm -rf build && mkdir build && cd build && CXXFLAGS=-fPIC cmake -DCASS_BUILD_INTEGRATION_TESTS=ON .. -DCMAKE_INSTALL_PREFIX=${CASSANDRA_DRIVER_ROOT} -DBUILD_SHARED_LIBS=ON -DBUILD_TESTING=OFF -DCMAKE_INSTALL_LIBDIR=lib -DLIBUV_ROOT_DIR=${LIBUV_ROOT} -DCASS_BUILD_EXAMPLES=ON)  # 指定配置指令（注意此处修改了安装目录，否则默认情况下回安装到系统目录）
set(CASSANDRA_DRIVER_MAKE         cd ${CASSANDRA_DRIVER_ROOT}/src/CASSANDRA_DRIVER/build && CC=gcc CXX=g++ CXXFLAGS=-fPIC make)  # 指定编译指令（需要覆盖默认指令，进入我们指定的CASSANDRA_DRIVER_ROOT目录下）
set(CASSANDRA_DRIVER_INSTALL      cd ${CASSANDRA_DRIVER_ROOT}/src/CASSANDRA_DRIVER && cd build && make install)  # 指定安装指令（需要覆盖默认指令，进入我们指定的CASSANDRA_DRIVER_ROOT目录下,可以copy 出来


# 指定编译好的静态库文件的路径
set(CASSANDRA_DRIVER_LIB_DIR       ${CASSANDRA_DRIVER_ROOT}/lib)
# 指定头文件所在的目录
set(CASSANDRA_DRIVER_INCLUDE_DIR   ${CASSANDRA_DRIVER_ROOT}/include)

message("add depends dirs: [${CASSANDRA_DRIVER_LIB_DIR}][${CASSANDRA_DRIVER_INCLUDE_DIR}]")
link_directories(${CASSANDRA_DRIVER_LIB_DIR})
include_directories(${CASSANDRA_DRIVER_INCLUDE_DIR})

message("cmake_module_path: " ${CMAKE_MODULE_PATH})

set(LIBUV_LIBRARY ${LIBUV_LIB_DIR})
set(LIBUV_INCLUDE_DIR   ${LIBUV_ROOT}/include)

message("aaaaaaaaaaaaaaaaaaaaaaaaaaaa" [${LIBUV_ROOT_DIR}][$ENV{LIBUV_ROOT_DIR}])

set(ENV{LIBUV_ROOT_DIR} "$ENV{LIBUV_ROOT_DIR}:${LIBUV_LIB_DIR}/")
set(LIBUV_ROOT_DIR "$ENV{LIBUV_ROOT_DIR}:${LIBUV_LIB_DIR}/")

if(NOT DEFINED ENV{LIBUV_ROOT_DIR})
    message(FATAL_ERROR "The LIBUV_ROOT_DIR environment variable is not set.")
endif()


list(FIND CMAKE_PREFIX_PATH ${CASSANDRA_DRIVER_LIB_DIR} INDEX)
if(INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${CASSANDRA_DRIVER_LIB_DIR})
endif()

message("CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH_STR}")

find_package(scylla-cpp-driver QUIET)
if (NOT scylla-cpp-driver_FOUND)
    message("not scylla-cpp-driverxxxxxxxxxxxxxxxxxxx")
else()
    message("find scylla-cpp-driverxxxxxxxxxxxxxxxxxxx")
endif()


if (NOT scylla-cpp-driver_FOUND)
        ExternalProject_Add(CASSANDRA_DRIVER
                #DEPENDS LIBUV
                PREFIX            ${CASSANDRA_DRIVER_ROOT}
                GIT_REPOSITORY    ${CASSANDRA_DRIVER_GIT_URL}
                GIT_TAG           ${CASSANDRA_DRIVER_GIT_TAG}
                CONFIGURE_COMMAND ${CASSANDRA_DRIVER_CONFIGURE}
                BUILD_COMMAND     ${CASSANDRA_DRIVER_MAKE}
                INSTALL_COMMAND   ${CASSANDRA_DRIVER_INSTALL}
                # LOG_CONFIGURE     1
                # LOG_INSTALL       1
                CMAKE_ARGS          "-DBUILD_SHARED_LIBS=ON;-DBUILD_TESTING=OFF;-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE};-DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH_STR};-DLIBUV_ROOT_DIR=${LIBUV_ROOT}"
        )
endif()

ADD_LIBRARY(scylla-cpp-driver SHARED IMPORTED GLOBAL)
SET_PROPERTY(TARGET scylla-cpp-driver PROPERTY IMPORTED_LOCATION ${CASSANDRA_DRIVER_LIB_DIR}/libscylla-cpp-driver.so)
add_dependencies(scylla-cpp-driver CASSANDRA_DRIVER libuv)

set(LIB_BIBRARY
    ${CASSANDRA_DRIVER_LIB_DIR}/libscylla-cpp-driver.so
    ${LIB_BIBRARY})
set(LIB_DEPENDS
    "scylla-cpp-driver"
    ${LIB_DEPENDS})

set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${CASSANDRA_DRIVER_ROOT}/lib/pkgconfig/")

# find_package(libuv)
# if(NOT LIBUV_FOUND)
#         message("libuv xxx not found" [${LIBUV_ROOT_DIR}] [${LIBUV_INCLUDE_DIR}] [${LIBUV_LIBRARIES}])
#         message(FATAL_ERROR "libuv xxx not found")
        
# endif()

