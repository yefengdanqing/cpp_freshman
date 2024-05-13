

include(ExternalProject)
set(EXTERNAL_LEVELDB_ROOT ${CMAKE_BINARY_DIR}/external/leveldb)
set(LEVELDB_GIT_TAG main)  # 指定版本
set(LEVELDB_GIT_URL https://github.com/google/leveldb.git)  # 指定git仓库地址

#
set(EXTERNAL_LEVELDB_ROOT ${CMAKE_BINARY_DIR}/external/leveldb)
set(LEVELDB_CONFIGURE    cd ${EXTERNAL_LEVELDB_ROOT}/src/LEVELDB && rm -fr build && mkdir build && cd build && CXXFLAGS=-fPIC cmake .. -DCMAKE_INSTALL_PREFIX=${EXTERNAL_LEVELDB_ROOT} -DCMAKE_INSTALL_LIBDIR=lib -DBUILD_SHARED_LIBS=ON -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS} -DHAVE_STD_REGEX=ON -DRUN_HAVE_STD_REGEX=1 -DLEVELDB_BUILD_TESTS=OFF -DLEVELDB_BUILD_BENCHMARKS=OFF) # 指定配置指令（注意此处修改了安装目录，否则默认情况下回安装到系统目录）
set(LEVELDB_MAKE         cd ${EXTERNAL_LEVELDB_ROOT}/src/LEVELDB/build && CXXFLAGS=-fPIC make -j8)  # 指定编译指令（需要覆盖默认指令，进入我们指定的LEVELDB_ROOT目录下）
set(LEVELDB_INSTALL      cd ${EXTERNAL_LEVELDB_ROOT}/src/LEVELDB/build && make install)  # 指定安装指令（需要覆盖默认指令，进入我们指定的LEVELDB_ROOT目录下,可以copy 出来
# 指定编译好的静态库文件的路径
set(LEVELDB_LIB_DIR       ${EXTERNAL_LEVELDB_ROOT}/lib)
# 指定头文件所在的目录
set(LEVELDB_INCLUDE_DIR   ${EXTERNAL_LEVELDB_ROOT}/include)

# list(FIND CMAKE_PREFIX_PATH ${LEVELDB_LIB_DIR} _DEP_INDEX)
# if (_DEP_INDEX EQUAL -1)
#     list(APPEND CMAKE_PREFIX_PATH ${LEVELDB_LIB_DIR})
# endif ()

list(FIND CMAKE_PREFIX_PATH ${EXTERNAL_LEVELDB_ROOT} _DEP_INDEX)
if (_DEP_INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${EXTERNAL_LEVELDB_ROOT})
endif ()
find_package(leveldb QUIET)

if (NOT leveldb_FOUND)
    ExternalProject_Add(LEVELDB
        PREFIX            ${EXTERNAL_LEVELDB_ROOT}
        GIT_REPOSITORY    ${LEVELDB_GIT_URL}
        GIT_TAG           ${LEVELDB_GIT_TAG}
        CONFIGURE_COMMAND ${LEVELDB_CONFIGURE}
        BUILD_COMMAND     ${LEVELDB_MAKE}
        INSTALL_COMMAND   ${LEVELDB_INSTALL}
        CMAKE_ARGS          -DCMAKE_POSITION_INDEPENDENT_CODE=ON
                            -DBUILD_SHARED_LIBS=ON
                            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                            -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
			                -DBUILD_TESTING=OFF
                            -DBENCHMARK_ENABLE_TESTING=OFF
			                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                            -DLEVELDB_BUILD_TESTS=OFF
                            -DLEVELDB_BUILD_BENCHMARKS=OFF
)
endif()


include_directories(${LEVELDB_INCLUDE_DIR})
link_directories(${LEVELDB_LIB_DIR})

ADD_LIBRARY(leveldb_leveldb SHARED IMPORTED GLOBAL)
SET_PROPERTY(TARGET leveldb_leveldb PROPERTY IMPORTED_LOCATION ${LEVELDB_LIB_DIR}/libleveldb.so)
add_dependencies(leveldb_leveldb LEVELDB)

set(LIB_BIBRARY
    ${LEVELDB_LIB_DIR}/libleveldb.so
    ${LIB_BIBRARY})
set(LIB_DEPENDS
        "leveldb_leveldb"
        ${LIB_DEPENDS})

