cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
include(ExternalProject)

set(THIRD_PARTY_PREFIX ${CMAKE_BINARY_DIR}/third_party)
set(FMT_ROOT ${THIRD_PARTY_PREFIX}/fmt)

set(FMT_LIB       ${FMT_ROOT}/lib)
set(FMT_INCLUDE_DIR   ${FMT_ROOT}/include)
set(FMT_LIBRARIES ${FMT_LIB}/libfmt.so)
include_directories(${FMT_INCLUDE_DIR})
link_directories(${FMT_LIB})


list(FIND CMAKE_PREFIX_PATH ${FMT_ROOT} INDEX)
if(INDEX EQUAL -1)
        list(APPEND CMAKE_PREFIX_PATH ${FMT_ROOT})
endif()

find_package(fmt QUIET)

if (NOT fmt_FOUND)
        ExternalProject_Add(FMT
                PREFIX                  ${FMT_ROOT}
                GIT_REPOSITORY          https://github.com/fmtlib/fmt.git
                GIT_TAG                 master
                CONFIGURE_COMMAND       cd ${FMT_ROOT}/src/FMT && cmake 
                                -D CMAKE_INSTALL_PREFIX=${FMT_ROOT} -DBUILD_SHARED_LIBS=ON 
                                -DCMAKE_INSTALL_LIBDIR=lib .
                BUILD_COMMAND           cd ${FMT_ROOT}/src/FMT && make -j8
                INSTALL_COMMAND         cd ${FMT_ROOT}/src/FMT && make install
        )
        # 将 fmt 库添加到链接目标的库
        #set(FMT_LIBRARIES ${FMT_LIB}/libfmt.so)
endif()
add_library(fmt SHARED IMPORTED GLOBAL)
add_dependencies(fmt FMT)
SET_PROPERTY(TARGET fmt PROPERTY IMPORTED_LOCATION ${FMT_LIBRARIES})


set(LIB_BIBRARY
        ${FMT_LIBRARIES}
        ${LIB_BIBRARY})
set(LIB_DEPENDS
        "fmt"
        ${LIB_DEPENDS})




