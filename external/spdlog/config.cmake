# set(EXTERNAL_PREFIX ${CMAKE_BINARY_DIR}/external)

# include(ExternalProject)
# message("start compile spdlog")

# set(EXTERNAL_SPDLOG_ROOT ${CMAKE_BINARY_DIR}/external/spdlog)
# set(SPDLOG_LIB       ${EXTERNAL_SPDLOG_ROOT}/lib/spdlog)
# set(SPDLOG_INCLUDE_DIR   ${EXTERNAL_SPDLOG_ROOT}/include)
# set(SPDLOG_LIBRARIES ${SPDLOG_LIB}/libspdlog.a)

# include_directories(${SPDLOG_INCLUDE_DIR})
# link_directories(${SPDLOG_LIB})

# set(SPDLOG_GIT_TAG  master)  # 指定版本
# set(SPDLOG_GIT_URL      git@github.com:gabime/spdlog.git)
# set(SPDLOG_CONFIGURE    cd ${EXTERNAL_SPDLOG_ROOT}/src/SDPLOG && cmake -DCMAKE_INSTALL_PREFIX=${EXTERNAL_SPDLOG_ROOT} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS} -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} .)
# set(SPDLOG_MAKE         cd ${EXTERNAL_SPDLOG_ROOT}/src/SDPLOG && make)
# set(SPDLOG_INSTALL      cd ${EXTERNAL_SPDLOG_ROOT}/src/SDPLOG && make install)
# #为啥一定是大写呢
# list(FIND CMAKE_PREFIX_PATH ${EXTERNAL_SPDLOG_ROOT} _DEP_INDEX)
# if (_DEP_INDEX EQUAL -1)
#     list(APPEND CMAKE_PREFIX_PATH ${EXTERNAL_SPDLOG_ROOT})
# endif ()

# find_package(spdlog QUIET)

# if (NOT spdlog_FOUND) 
#         ExternalProject_Add(SDPLOG
#                 PREFIX            ${EXTERNAL_SPDLOG_ROOT}
#                 GIT_REPOSITORY    ${SPDLOG_GIT_URL}
#                 GIT_TAG           ${SPDLOG_GIT_TAG}
#                 CONFIGURE_COMMAND ${SPDLOG_CONFIGURE}
#                 BUILD_COMMAND     ${SPDLOG_MAKE}
#                 INSTALL_COMMAND   ${SPDLOG_INSTALL}
#         )
# endif()

# ADD_LIBRARY(spdlog STATIC IMPORTED GLOBAL)
# SET_PROPERTY(TARGET spdlog PROPERTY IMPORTED_LOCATION ${SPDLOG_LIBRARIES})
# # add_library(glog gflags)
# add_dependencies(spdlog SDPLOG)
# set(LIB_BIBRARY
#         ${SPDLOG_LIBRARIES}
#         ${LIB_BIBRARY})
# set(LIB_DEPENDS
#         "spdlog"
#         ${LIB_DEPENDS})

set(EXTERNAL_PREFIX ${CMAKE_BINARY_DIR}/external)

include(ExternalProject)
message(STATUS "Start compiling spdlog")

# 设置 spdlog 路径（统一小写）
set(SPDLOG_ROOT ${EXTERNAL_PREFIX}/spdlog)
set(SPDLOG_INCLUDE_DIR ${SPDLOG_ROOT}/include)
set(SPDLOG_LIBRARY ${SPDLOG_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}spdlog${CMAKE_STATIC_LIBRARY_SUFFIX})

# 添加包含和链接目录
include_directories(${SPDLOG_INCLUDE_DIR})
link_directories(${SPDLOG_ROOT}/lib)

# 检查是否已存在
list(FIND CMAKE_PREFIX_PATH ${SPDLOG_ROOT} _spdlog_index)
if (_spdlog_index EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${SPDLOG_ROOT})
endif()

# 使用 CMake 原生方式配置 ExternalProject
if (NOT TARGET spdlog)
    ExternalProject_Add(
        spdlog_project  # 项目名称（任意）
        PREFIX          ${SPDLOG_ROOT}
        GIT_REPOSITORY  https://github.com/gabime/spdlog.git
        GIT_TAG         v1.11.0  # 建议指定稳定版本
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${SPDLOG_ROOT}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DSPDLOG_BUILD_EXAMPLE=OFF
            -DSPDLOG_BUILD_TESTS=OFF
            -DSPDLOG_BUILD_SHARED=OFF  # 强制静态库
        BUILD_BYPRODUCTS ${SPDLOG_LIBRARY}
    )

    # 导入库目标
    add_library(spdlog STATIC IMPORTED)
    set_target_properties(spdlog PROPERTIES
        IMPORTED_LOCATION ${SPDLOG_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${SPDLOG_INCLUDE_DIR}
    )
    add_dependencies(spdlog spdlog_project)
endif()

# 添加到全局变量
list(APPEND LIB_BIBRARY ${SPDLOG_LIBRARY})
list(APPEND LIB_DEPENDS spdlog)