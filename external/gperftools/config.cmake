# INCLUDE(ExternalProject)

# set(GPERFTOOLS_SOURCE_DIR "${EXTERNAL_PREFIX}/gperftools/")
# set(GPERFTOOLS_INSTALL_DIR "${EXTERNAL_PREFIX}/gperftools")
# set(GPERFTOOLS_INCLUDE_DIR "${GPERFTOOLS_INSTALL_DIR}/include")
# set(GPERFTOOLS_LIBRAYIES "${GPERFTOOLS_INSTALL_DIR}/lib/libtcmalloc.a"
#                          "${GPERFTOOLS_INSTALL_DIR}/lib/libprofiler.a"
#                          "${GPERFTOOLS_INSTALL_DIR}/lib/libtcmalloc_and_profiler.a")
# #"${GPERFTOOLS_INSTALL_DIR}/lib/libtcmalloc_and_profiler.a"

# set(_DEP_VER 2.7)
# set(_DEP_URL https://github.com/gperftools/gperftools/archive/gperftools-2.7.tar.gz)
                         

# include_directories(SYSTEM ${GPERFTOOLS_INCLUDE_DIR})
# #include_directories(SYSTEM ${GPERFTOOLS_SOURCE_DIR}/src/gperftools_gperftools/src/base)

# ExternalProject_Add(
#     gperftools_gperftools
#     GIT_REPOSITORY  "https://github.com/gperftools/gperftools.git"
#     GIT_TAG         master
#     PREFIX          ${GPERFTOOLS_SOURCE_DIR}
#     UPDATE_COMMAND  ""
#     CMAKE_ARGS      -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#                     -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#                     -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
#                     -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
#                     -DCMAKE_PREFIX_PATH=${GPERFTOOLS_SOURCE_DIR}
#                     -DCMAKE_INSTALL_PREFIX=${GPERFTOOLS_INSTALL_DIR}
#                     -DCMAKE_POSITION_INDEPENDENT_CODE=ON
# 		            -DBUILD_SHARED_LIBS=OFF
#                     -DBUILD_STATIC_LIBS=ON
#                     -DCMAKE_INSTALL_LIBDIR=lib
#                     -DNO_TCMALLOC_SAMPLES=ON
#                     -DGPERFTOOLS_BUILD_STATIC=ON
# )
# message("GPERFTOOLS_LIBRAYIES=${GPERFTOOLS_LIBRAYIES}")
# add_library(gperftools STATIC IMPORTED GLOBAL)
# set_property(TARGET gperftools PROPERTY IMPORTED_LOCATION ${GPERFTOOLS_LIBRAYIES})
# #set_property(TARGET gperftools PROPERTY IMPORTED_LOCATION_RELEASE ${GPERFTOOLS_LIBRAYIES})
# add_dependencies(gperftools gperftools_gperftools)




include(ExternalProject)

set(GPERFTOOLS_SOURCE_DIR "${EXTERNAL_PREFIX}/gperftools")
set(GPERFTOOLS_INSTALL_DIR "${EXTERNAL_PREFIX}/gperftools")
set(GPERFTOOLS_INCLUDE_DIR "${GPERFTOOLS_INSTALL_DIR}/include")
set(GPERFTOOLS_LIB_DIR "${GPERFTOOLS_INSTALL_DIR}/lib")
set(GPERFTOOLS_LIBRAYIES "${GPERFTOOLS_INSTALL_DIR}/lib/libtcmalloc.a"
                         "${GPERFTOOLS_INSTALL_DIR}/lib/libprofiler.a"
                         "${GPERFTOOLS_INSTALL_DIR}/lib/libtcmalloc_and_profiler.a")



set(GPERFTOOLS_GIT_TAG gperftools-2.15)  # 指定版本
set(GPERFTOOLS_GIT_URL https://github.com/gperftools/gperftools.git)  # 指定git仓库地址



set(GPERFTOOLS_CONFIGURE    cd ${GPERFTOOLS_SOURCE_DIR}/src/gperftools_gperftools && autoreconf -i -m && ./configure --prefix=${GPERFTOOLS_INSTALL_DIR} --enable-shared=no --enable-static=yes --enable-frame-pointers --libdir=${GPERFTOOLS_LIB_DIR})

link_directories(${GPERFTOOLS_LIB_DIR})
include_directories(${GPERFTOOLS_INCLUDE_DIR})
list(FIND CMAKE_PREFIX_PATH ${GPERFTOOLS_INSTALL_DIR} _DEP_INDEX)
if (_DEP_INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${GPERFTOOLS_INSTALL_DIR})
endif ()

set(GPERFTOOLS_MAKE         cd ${GPERFTOOLS_SOURCE_DIR}/src/gperftools_gperftools && make -j8)  # 指定编译指令（需要覆盖默认指令，进入我们指定的GPERFTOOLS_ROOT目录下）
set(GPERFTOOLS_INSTALL      cd ${GPERFTOOLS_SOURCE_DIR}/src/gperftools_gperftools && make install)  # 指定安装指令（需要覆盖默认指令，进入我们指定的GPERFTOOLS_ROOT目录下,可以copy 出来
 # 如果不存在，则需要编译,要先创建目标
find_package(gperftools QUIET)
if(NOT gperftools_FOUND)
        ExternalProject_Add(gperftools_gperftools
                PREFIX            ${GPERFTOOLS_SOURCE_DIR}          
                GIT_REPOSITORY    ${GPERFTOOLS_GIT_URL}
                GIT_TAG           ${GPERFTOOLS_GIT_TAG}
                CONFIGURE_COMMAND ${GPERFTOOLS_CONFIGURE}
                BUILD_COMMAND     ${GPERFTOOLS_MAKE}
                INSTALL_COMMAND   ${GPERFTOOLS_INSTALL}
                # LOG_CONFIGURE     1
                # LOG_INSTALL       1
                CMAKE_ARGS      -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                                -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                                -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
                                -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
                                -DCMAKE_PREFIX_PATH=${GPERFTOOLS_SOURCES_DIR}
                                -DCMAKE_INSTALL_PREFIX=${GPERFTOOLS_ROOT/skt}
                                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
		                        -DBUILD_SHARED_LIBS=OFF
                                -DBUILD_STATIC_LIBS=ON
                                -DWITH_GFLAGS=ON
                                -DBUILD_GMOCK=OFF
                                -DCMAKE_INSTALL_LIBDIR=lib
        )
endif()
ADD_LIBRARY(gperftools STATIC IMPORTED GLOBAL)
SET_PROPERTY(TARGET gperftools PROPERTY IMPORTED_LOCATION ${GPERFTOOLS_LIB_DIR}/libtcmalloc.a)
add_dependencies(gperftools gperftools_gperftools)

set(LIB_BIBRARY
    ${GPERFTOOLS_LIBRAYIES}
    ${LIB_BIBRARY})
set(LIB_DEPENDS
        "gperftools"
        ${LIB_DEPENDS})



