# include(${CMAKE_CURRENT_LIST_DIR}/../base.cmake)

# set(BOOST_ENABLE_CMAKE ON)
# set(FETCHCONTENT_QUIET FALSE)

# include(FetchContent)

# FetchContent_Declare(gflags
# 	GIT_REPOSITORY	https://github.com/gflags/gflags.git
# 	GIT_TAG			master
# )

# FetchContent_MakeAvailable(gflags)


# include_directories(${gflags_BINARY_DIR}/include)
# message("skt gflags dir ${gflags_SOURCE_DIR} ${gflags_BINARY_DIR}")
# link_directories(${gflags_BINARY_DIR})
# message("module path: ${CMAKE_MODULE_PATH}")
# #set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${gflags_BINARY_DIR})
# message("module path: ${CMAKE_MODULE_PATH}")






# # INCLUDE(ExternalProject)

# # set(GFLAGS_INSTALL_DIR "${PROJECT_BINARY_DIR}/external/gflags")
# # set(GFLAGS_INCLUDE_DIR "${GFLAGS_INSTALL_DIR}/include" CACHE PATH "gflags include directory." FORCE)
# # set(GFLAGS_LIBRARIES "${GFLAGS_INSTALL_DIR}/lib/libgflags.a" CACHE FILEPATH "GFLAGS_LIBRARIES" FORCE)

# # include_directories(SYSTEM ${GFLAGS_INCLUDE_DIR})

# # execute_process(
# #     COMMAND bash -c "tar -xvzf ${PROJ_EXTERNAL_DIR}/gflags/gflags-2.2.2.tar.gz -C ${PROJ_EXTERNAL_UNZIP_DIR} && patch -d ${PROJ_EXTERNAL_UNZIP_DIR}/gflags-2.2.2 -p0 < ${PROJ_EXTERNAL_DIR}/gflags/mod.diff"
# #     RESULT_VARIABLE GFLAGS_SOURCE_UNZIP_SUCCESS
# # )

# # if(NOT "${GFLAGS_SOURCE_UNZIP_SUCCESS}" STREQUAL "0")
# #     message(FATAL_ERROR "unzip && patch tar failed -> ${GFLAGS_SOURCE_UNZIP_SUCCESS}")
# # endif()

# # ExternalProject_Add(
# #   gflags_gflags
# #   SOURCE_DIR       "${PROJ_EXTERNAL_UNZIP_DIR}/gflags-2.2.2"
# #   BUILD_BYPRODUCTS ${GFLAGS_LIBRARIES}
# #   CMAKE_ARGS       -DCMAKE_INSTALL_PREFIX=${GFLAGS_INSTALL_DIR}
# #                    -DBUILD_STATIC_LIBS=ON
# #                    -DBUILD_TESTING=OFF
# #                    -DGFLAGS_NAMESPACE=google
# # 		               -DCMAKE_POSITION_INDEPENDENT_CODE=ON
# #                    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
# #                    -DCMAKE_GENERATOR=${CMAKE_GENERATOR}
# #                    ${EXTERNAL_EXTRA_CMAKE_ARGS}
# #   BUILD_IN_SOURCE 1
# # )

# # add_library(gflags STATIC IMPORTED GLOBAL)
# # set_property(TARGET gflags PROPERTY IMPORTED_LOCATION ${GFLAGS_LIBRARIES})
# # add_dependencies(gflags gflags_gflags)
# find_package(Glog)
# if (GFLAGS_FOUND)
#     set(GFLAGS_EXTERNAL FALSE)
# endif()


include(ExternalProject)
set(EXTERNAL_GFLAGS_ROOT ${CMAKE_BINARY_DIR}/external/gflags)
# 指定编译好的静态库文件的路径
set(GFLAGS_LIB_DIR       ${EXTERNAL_GFLAGS_ROOT}/lib)
# 指定头文件所在的目录
set(GFLAGS_INCLUDE_DIR   ${EXTERNAL_GFLAGS_ROOT}/include)
set(GFLAGS_SELF_LIBRARY "${GFLAGS_LIB_DIR}/libgflags.so" CACHE FILEPATH "GFLAGS_LIBRARIES" FORCE)

set(GFLAGS_GIT_TAG  master)  # 指定版本
set(GFLAGS_GIT_URL https://github.com/gflags/gflags.git)  # 指定git仓库地址
set(EXTERNAL_PREFIX ${CMAKE_BINARY_DIR}/external)
#
set(GFLAGS_CONFIGURE    cd ${EXTERNAL_GFLAGS_ROOT}/src/GFLAGS && rm -fr build && mkdir build && cd build && CXXFLAGS=-fPIC cmake .. -DCMAKE_INSTALL_PREFIX=${EXTERNAL_GFLAGS_ROOT} -DBUILD_SHARED_LIBS=ON -DBUILD_TESTING=OFF)  # 指定配置指令（注意此处修改了安装目录，否则默认情况下回安装到系统目录）
# set(GFLAGS_MAKE         cd ${EXTERNAL_GFLAGS_ROOT}/src/GFLAGS/build && CC=gcc CXX=g++ CXXFLAGS=-fPIC make -j 8)  # 指定编译指令（需要覆盖默认指令，进入我们指定的GFLAGS_ROOT目录下）
# set(GFLAGS_INSTALL      cd ${EXTERNAL_GFLAGS_ROOT}/src/GFLAGS && cd build && make install)  # 指定安装指令（需要覆盖默认指令，进入我们指定的GFLAGS_ROOT目录下,可以copy 出来


#set(GFLAGS_CONFIGURE    cd ${EXTERNAL_GFLAGS_ROOT}/src/GFLAGS && rm -fr build && mkdir build && cd build && CC=gcc CXX=g++ CXXFLAGS=-fPIC cmake .. -DCMAKE_INSTALL_PREFIX=${EXTERNAL_PREFIX} -DBUILD_SHARED_LIBS=ON)  # 指定配置指令（注意此处修改了安装目录，否则默认情况下回安装到系统目录）
set(GFLAGS_MAKE         cd ${EXTERNAL_GFLAGS_ROOT}/src/GFLAGS/build && CC=gcc CXX=g++ CXXFLAGS=-fPIC make)  # 指定编译指令（需要覆盖默认指令，进入我们指定的GFLAGS_ROOT目录下）
set(GFLAGS_INSTALL      cd ${EXTERNAL_GFLAGS_ROOT}/src/GFLAGS && cd build && make install)  # 指定安装指令（需要覆盖默认指令，进入我们指定的GFLAGS_ROOT目录下,可以copy 出来

link_directories(${GFLAGS_LIB_DIR})
include_directories(${GFLAGS_INCLUDE_DIR})

# list(FIND CMAKE_PREFIX_PATH ${GFLAGS_LIB_DIR} INDEX)
# if(INDEX EQUAL -1)
#     list(APPEND CMAKE_PREFIX_PATH ${GFLAGS_LIB_DIR})
# endif()

list(FIND CMAKE_PREFIX_PATH ${EXTERNAL_GFLAGS_ROOT} INDEX)
if(INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${EXTERNAL_GFLAGS_ROOT})
endif()

find_package(GFLAGS QUIET)

if (NOT GFLAGS_FOUND)
        ExternalProject_Add(GFLAGS
                PREFIX            ${EXTERNAL_GFLAGS_ROOT}
                GIT_REPOSITORY    ${GFLAGS_GIT_URL}
                GIT_TAG           ${GFLAGS_GIT_TAG}
                CONFIGURE_COMMAND ${GFLAGS_CONFIGURE}
                BUILD_COMMAND     ${GFLAGS_MAKE}
                INSTALL_COMMAND   ${GFLAGS_INSTALL}
                # LOG_CONFIGURE     1
                # LOG_INSTALL       1
                CMAKE_ARGS      -DBUILD_SHARED_LIBS=ON
			        -DBUILD_TESTING=OFF
			        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
			        -DCMAKE_INSTALL_PREFIX=${GFLAGS_INSTALL_DIR}
                    -DGFLAGS_NAMESPACE=google
)
endif()

ADD_LIBRARY(gflags_gflags SHARED IMPORTED GLOBAL)
SET_PROPERTY(TARGET gflags_gflags PROPERTY IMPORTED_LOCATION ${GFLAGS_SELF_LIBRARY})
add_dependencies(gflags_gflags GFLAGS)

set(LIB_BIBRARY
    ${GFLAGS_SELF_LIBRARY}
    ${LIB_BIBRARY})
set(LIB_DEPENDS
        "gflags_gflags"
        ${LIB_DEPENDS})


