


include(ExternalProject)
set(THIRD_PARTY_PREFIX ${CMAKE_BINARY_DIR}/third_party)
set(LIBUV_INCLUDE_DIR "${LIBUV_INSTALL_DIR}/include" CACHE PATH "libuv include directory." FORCE)
set(LIBUV_LIBRARIES "${LIBUV_INSTALL_DIR}/lib/Libuv.a" CACHE FILEPATH "LIBUV_LIBRARIES" FORCE)

set(LIBUV_ROOT ${THIRD_PARTY_PREFIX}/libuv)
set(LIBUV_ROOT_DIR "${THIRD_PARTY_PREFIX}" CACHE STRING "libuv root directory" FORCE)
set(LIBUV_GIT_TAG v1.x)  # 指定版本
set(LIBUV_GIT_URL https://github.com/libuv/libuv.git)  # 指定git仓库地址


set(LIBUV_CONFIGURE    cd ${LIBUV_ROOT}/src/LIBUV && rm -fr build && mkdir build && cd build && CXXFLAGS=-fPIC cmake .. -DCMAKE_INSTALL_PREFIX=${LIBUV_ROOT} -DBUILD_SHARED_LIBS=ON -DBUILD_TESTING=OFF -DCMAKE_INSTALL_LIBDIR=lib)  # 指定配置指令（注意此处修改了安装目录，否则默认情况下回安装到系统目录）
set(LIBUV_MAKE         cd ${LIBUV_ROOT}/src/LIBUV/build && CC=gcc CXX=g++ CXXFLAGS=-fPIC make)  # 指定编译指令（需要覆盖默认指令，进入我们指定的LIBUV_ROOT目录下）
set(LIBUV_INSTALL      cd ${LIBUV_ROOT}/src/LIBUV && cd build && make install)  # 指定安装指令（需要覆盖默认指令，进入我们指定的LIBUV_ROOT目录下,可以copy 出来


# 指定编译好的静态库文件的路径
set(LIBUV_LIB_DIR       ${LIBUV_ROOT}/lib)
# 指定头文件所在的目录
set(LIBUV_INCLUDE_DIR   ${LIBUV_ROOT}/include)

set(LIBUV_LIBRARY ${LIBUV_LIB_DIR}/libuv.so)

link_directories(${LIBUV_LIB_DIR})
include_directories(${LIBUV_INCLUDE_DIR})

list(FIND CMAKE_PREFIX_PATH ${LIBUV_ROOT} INDEX)
if(INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${LIBUV_ROOT})
endif()
list(APPEND CMAKE_PREFIX_PATH "\;${LIBUV_LIB_DIR}/cmake/libuv")

# for FindLibUV in cassandra-cpp-driver
set(ENV{LIBUV_ROOT_DIR} "$ENV{LIBUV_ROOT_DIR}:${LIBUV_LIB_DIR}")
# for pkg config path
set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${LIBUV_ROOT}/lib/pkgconfig/")
find_package(PkgConfig REQUIRED)
#pkg_search_module(LIBUV REQUIRED libuv)
find_package(uv QUIET)

if (NOT uv_FOUND) 
        ExternalProject_Add(LIBUV
                #DEPENDS LIBUV
                PREFIX            ${LIBUV_ROOT}
                GIT_REPOSITORY    ${LIBUV_GIT_URL}
                GIT_TAG           ${LIBUV_GIT_TAG}
                CONFIGURE_COMMAND ${LIBUV_CONFIGURE}
                BUILD_COMMAND     ${LIBUV_MAKE}
                INSTALL_COMMAND   ${LIBUV_INSTALL}
                # LOG_CONFIGURE     1
                # LOG_INSTALL       1
                CMAKE_ARGS          -DBUILD_SHARED_LIBS=ON
			    -DBUILD_TESTING=OFF
			    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
			    -DCMAKE_INSTALL_PREFIX=${LIBUV_ROOT}
                            -DCMAKE_INSTALL_LIBDIR=lib
                            -DCMAKE_PREFIX_PATH=${}
        )
endif()
ADD_LIBRARY(libuv SHARED IMPORTED GLOBAL)
SET_PROPERTY(TARGET libuv PROPERTY IMPORTED_LOCATION ${LIBUV_LIB_DIR}/libuv.so)
#SET_PROPERTY(TARGET LIBUV PROPERTY IMPORTED_LOCATION ${LIBUV_LIB_DIR}/libLIBUV.a)
add_dependencies(libuv LIBUV)

set(LIB_BIBRARY
    ${LIBUV_LIBRARIES}
    ${LIB_BIBRARY})
set(LIB_DEPENDS
        "libuv"
        ${LIB_DEPENDS})

message("cmake_module_path: " ${CMAKE_MODULE_PATH})
list(APPEND CMAKE_MODULE_PATH "\;${LIBUV_LIB_DIR}/cmake/libuv")


