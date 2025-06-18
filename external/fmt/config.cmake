cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
# include(ExternalProject)

# set(EXTERNAL_PREFIX ${CMAKE_BINARY_DIR}/external)
# set(EXTERNAL_FMT_ROOT ${EXTERNAL_PREFIX}/fmt)

# set(FMT_LIB       ${EXTERNAL_FMT_ROOT}/lib)
# set(FMT_INCLUDE_DIR   ${EXTERNAL_FMT_ROOT}/include)
# set(FMT_LIBRARIES ${FMT_LIB}/libfmt.so)
# include_directories(${FMT_INCLUDE_DIR})
# link_directories(${FMT_LIB})


# list(FIND CMAKE_PREFIX_PATH ${EXTERNAL_FMT_ROOT} INDEX)
# if(INDEX EQUAL -1)
#         list(APPEND CMAKE_PREFIX_PATH ${EXTERNAL_FMT_ROOT})
# endif()

# find_package(fmt QUIET)

#         ExternalProject_Add(FMT
#                 PREFIX                  ${EXTERNAL_FMT_ROOT}
#                 GIT_REPOSITORY          git@github.com:fmtlib/fmt.git
#                 GIT_TAG                 master
#                 CONFIGURE_COMMAND       cd ${EXTERNAL_FMT_ROOT}/src/FMT && cmake 
#                                 -D CMAKE_INSTALL_PREFIX=${EXTERNAL_FMT_ROOT} -DBUILD_SHARED_LIBS=ON 
#                                 -DCMAKE_INSTALL_LIBDIR=lib .
#                 BUILD_COMMAND           cd ${EXTERNAL_FMT_ROOT}/src/FMT && make -j8
#                 INSTALL_COMMAND         cd ${EXTERNAL_FMT_ROOT}/src/FMT && make install
#         )
#         # 将 fmt 库添加到链接目标的库
#         #set(FMT_LIBRARIES ${FMT_LIB}/libfmt.so)
# add_library(fmt SHARED IMPORTED GLOBAL)
# add_dependencies(fmt FMT)
# SET_PROPERTY(TARGET fmt PROPERTY IMPORTED_LOCATION ${FMT_LIBRARIES})







# include(FetchContent)

# FetchContent_Declare(fmt
# 	GIT_REPOSITORY	git@github.com:fmt/fmt.git
# 	GIT_TAG			master
# )

# FetchContent_MakeAvailable(fmt)


# include_directories(${fmt_BINARY_DIR}/include)
# message("skt fmt dir ${fmt_SOURCE_DIR} ${fmt_BINARY_DIR}")
# link_directories(${fmt_BINARY_DIR})
# message("module path: ${CMAKE_MODULE_PATH}")
# #set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${fmt_BINARY_DIR})
# message("module path: ${CMAKE_MODULE_PATH}")




include(ExternalProject)
set(EXTERNAL_FMT_ROOT ${CMAKE_BINARY_DIR}/external/fmt)
# 指定编译好的静态库文件的路径
set(FMT_LIB_DIR       ${EXTERNAL_FMT_ROOT}/lib)
# 指定头文件所在的目录
set(FMT_INCLUDE_DIR   ${EXTERNAL_FMT_ROOT}/include)
set(FMT_SELF_LIBRARY "${FMT_LIB_DIR}/libfmt.a" CACHE FILEPATH "FMT_LIBRARIES" FORCE)

set(FMT_GIT_TAG  master)  # 指定版本
set(FMT_GIT_URL git@github.com:fmtlib/fmt.git)  # 指定git仓库地址
#
set(FMT_CONFIGURE    cd ${EXTERNAL_FMT_ROOT}/src/FMT && rm -fr build && mkdir build && cd build && CXXFLAGS=-fPIC cmake .. -DCMAKE_INSTALL_PREFIX=${EXTERNAL_FMT_ROOT} -DBUILD_SHARED_LIBS=ON -DBUILD_TESTING=OFF)  # 指定配置指令（注意此处修改了安装目录，否则默认情况下回安装到系统目录）

set(FMT_MAKE         cd ${EXTERNAL_FMT_ROOT}/src/FMT/build && CC=gcc CXX=g++ CXXFLAGS=-fPIC make)  # 指定编译指令（需要覆盖默认指令，进入我们指定的FMT_ROOT目录下）
set(FMT_INSTALL      cd ${EXTERNAL_FMT_ROOT}/src/FMT && cd build && make install)  # 指定安装指令（需要覆盖默认指令，进入我们指定的FMT_ROOT目录下,可以copy 出来

link_directories(${FMT_LIB_DIR})
include_directories(${FMT_INCLUDE_DIR})


list(FIND CMAKE_PREFIX_PATH ${EXTERNAL_FMT_ROOT} INDEX)
if(INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${EXTERNAL_FMT_ROOT})
endif()

# find_package(FMT QUIET)

# if (NOT FMT_FOUND)
#         ExternalProject_Add(FMT
#                 PREFIX            ${EXTERNAL_FMT_ROOT}
#                 GIT_REPOSITORY    ${FMT_GIT_URL}
#                 GIT_TAG           ${FMT_GIT_TAG}
#                 CONFIGURE_COMMAND ${FMT_CONFIGURE}
#                 BUILD_COMMAND     ${FMT_MAKE}
#                 INSTALL_COMMAND   ${FMT_INSTALL}
#                 # LOG_CONFIGURE     1
#                 # LOG_INSTALL       1
#                 CMAKE_ARGS      -DBUILD_SHARED_LIBS=ON
# 			        -DBUILD_TESTING=OFF
# 			        -DCMAKE_INSTALL_PREFIX=${EXTERNAL_FMT_ROOT}
# )
# endif()

    ExternalProject_Add(FMT
        GIT_REPOSITORY        ${FMT_GIT_URL}
        GIT_TAG               ${FMT_GIT_TAG}
        # PREFIX                  "${EXTERNAL_BRPC_ROOT}" #debug code
        SOURCE_DIR            "${EXTERNAL_FMT_ROOT}"
        INSTALL_DIR           "${EXTERNAL_FMT_ROOT}"
        UPDATE_COMMAND ""
        # PATCH_COMMAND     
        #     COMMAND   bash -c "set -ex && git apply --check ${PROJECT_SOURCE_DIR}/external/brpc/922.patch.new && git apply ${PROJECT_SOURCE_DIR}/external/brpc/922.patch.new"
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_FMT_ROOT}
                -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
                -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
                -DCMAKE_INSTALL_LIBDIR=${FMT_LIB_DIR}
                -DCMAKE_PREFIX_PATH=${EXTERNAL_FMT_ROOT}
                -DBUILD_UNIT_TESTS=OFF
                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                -DBUILD_SHARED_LIBS=OFF
        LIST_SEPARATOR  |
        BUILD_BYPRODUCTS ${FMT_SELF_LIBRARY})

ADD_LIBRARY(fmt_fmt SHARED IMPORTED GLOBAL)
SET_PROPERTY(TARGET fmt_fmt PROPERTY IMPORTED_LOCATION ${FMT_SELF_LIBRARY})
add_dependencies(fmt_fmt FMT)

set(LIB_BIBRARY
    ${FMT_SELF_LIBRARY}
    ${LIB_BIBRARY})
set(LIB_DEPENDS
        "fmt_fmt"
        ${LIB_DEPENDS})






