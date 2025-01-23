include(ExternalProject)
set(CMAKE_CXX_STANDARD 17)
set(EXTERNAL_BRPC_ROOT ${CMAKE_BINARY_DIR}/external/brpc)
set(BRPC_GIT_TAG 1.10.0)
set(BRPC_GIT_URL https://github.com/apache/incubator-brpc.git)


set(BRPC_LIB_DIR       ${EXTERNAL_BRPC_ROOT}/lib)
set(BRPC_INCLUDE_DIR   ${EXTERNAL_BRPC_ROOT}/include)
set(BRPC_LIBRARIES   "${BRPC_LIB_DIR}/libbrpc.a" CACHE FILEPATH "BRPC_LIBRARIES" FORCE)
include_directories(${BRPC_INCLUDE_DIR})
link_directories(${BRPC_LIB_DIR})

list(FIND CMAKE_PREFIX_PATH ${EXTERNAL_BRPC_ROOT} _DEP_INDEX)
if (_DEP_INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${EXTERNAL_BRPC_ROOT})
endif ()
string(REPLACE ";" "|" TBRPC_CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}")

#find_package(brpc QUIET)
if (NOT brpc_FOUND)
    ExternalProject_Add(BRPC
        GIT_REPOSITORY        ${BRPC_GIT_URL}
        GIT_TAG               ${BRPC_GIT_TAG}
        # PREFIX                  "${EXTERNAL_BRPC_ROOT}" #debug code
        SOURCE_DIR            "${EXTERNAL_BRPC_ROOT}"
        DEPENDS               openssl_openssl leveldb_leveldb gflags_gflags protobuf_protobuf
        INSTALL_DIR           "${EXTERNAL_BRPC_ROOT}"
        UPDATE_COMMAND ""
        # PATCH_COMMAND     
        #     COMMAND   bash -c "set -ex && git apply --check ${PROJECT_SOURCE_DIR}/external/brpc/922.patch.new && git apply ${PROJECT_SOURCE_DIR}/external/brpc/922.patch.new"
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_BRPC_ROOT}
                -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
                -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
                -DCMAKE_INSTALL_LIBDIR=${EXTERNAL_BRPC_ROOT}/lib
                -DCMAKE_PREFIX_PATH=${TBRPC_CMAKE_PREFIX_PATH}
                -DWITH_GLOG=OFF
                -DBUILD_BRPC_TOOLS=OFF
                -DBUILD_UNIT_TESTS=OFF
                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                -DBUILD_SHARED_LIBS=OFF
                -DBRPC_ENABLE_CPU_PROFILER=ON
                #-DProtobuf_PROTOC_EXECUTABLE=${PBUF_PROTOC}
        LIST_SEPARATOR  |
        BUILD_BYPRODUCTS ${BRPC_LIBRARIES})
endif()


ADD_LIBRARY(brpc STATIC IMPORTED GLOBAL)
SET_PROPERTY(TARGET brpc PROPERTY IMPORTED_LOCATION ${BRPC_LIBRARIES} $PROTOBUF_SELF_LIBRARIES)
add_dependencies(brpc BRPC ${LIB_DEPENDS})
set(LIB_BIBRARY
    ${BRPC_LIBRARIES}
    ${LIB_BIBRARY})

set(LIB_DEPENDS
        "brpc"
        ${LIB_DEPENDS})

    
# https://github.com/PaddlePaddle/Paddle/blob/develop/cmake/external/brpc.cmake
#https://github.com/tushushu/bigflow/tree/ab494e49a02b446bb2f504a2652f866c924c1baf/cmake

