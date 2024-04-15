set(EXTERNAL_PREFIX ${CMAKE_BINARY_DIR}/external)

include(ExternalProject)
message("start compile spdlog")

set(EXTERNAL_SPDLOG_ROOT ${CMAKE_BINARY_DIR}/external/spdlog)
set(SPDLOG_LIB       ${EXTERNAL_SPDLOG_ROOT}/lib/spdlog)
set(SPDLOG_INCLUDE_DIR   ${EXTERNAL_SPDLOG_ROOT}/include)
set(SPDLOG_LIBRARIES ${SPDLOG_LIB}/libspdlog.a)

include_directories(${SPDLOG_INCLUDE_DIR})
link_directories(${SPDLOG_LIB})

set(SPDLOG_GIT_TAG  v1.4.1)  # 指定版本
set(SPDLOG_GIT_URL      https://github.com/gabime/spdlog.git)
set(SPDLOG_CONFIGURE    cd ${EXTERNAL_SPDLOG_ROOT}/src/SDPLOG && cmake -DCMAKE_INSTALL_PREFIX=${EXTERNAL_SPDLOG_ROOT} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS} -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} .)
set(SPDLOG_MAKE         cd ${EXTERNAL_SPDLOG_ROOT}/src/SDPLOG && make)
set(SPDLOG_INSTALL      cd ${EXTERNAL_SPDLOG_ROOT}/src/SDPLOG && make install)
#为啥一定是大写呢
list(FIND CMAKE_PREFIX_PATH ${EXTERNAL_SPDLOG_ROOT} _DEP_INDEX)
if (_DEP_INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${EXTERNAL_SPDLOG_ROOT})
endif ()

find_package(spdlog QUIET)

if (NOT spdlog_FOUND) 
        ExternalProject_Add(SDPLOG
                PREFIX            ${EXTERNAL_SPDLOG_ROOT}
                GIT_REPOSITORY    ${SPDLOG_GIT_URL}
                GIT_TAG           ${SPDLOG_GIT_TAG}
                CONFIGURE_COMMAND ${SPDLOG_CONFIGURE}
                BUILD_COMMAND     ${SPDLOG_MAKE}
                INSTALL_COMMAND   ${SPDLOG_INSTALL}
        )
endif()

ADD_LIBRARY(spdlog STATIC IMPORTED GLOBAL)
SET_PROPERTY(TARGET spdlog PROPERTY IMPORTED_LOCATION ${SPDLOG_LIBRARIES})
# add_library(glog gflags)
add_dependencies(spdlog SDPLOG)
set(LIB_BIBRARY
        ${SPDLOG_LIBRARIES}
        ${LIB_BIBRARY})
set(LIB_DEPENDS
        "spdlog"
        ${LIB_DEPENDS})