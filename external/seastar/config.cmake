#include(${CMAKE_CURRENT_LIST_DIR}/../base.cmake)
#include(${CMAKE_CURRENT_LIST_DIR}/../protobuf/config.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../boost/config.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../fmt/config.cmake)

set(THIRD_PARTY_PREFIX ${CMAKE_BINARY_DIR}/third_party)

include(ExternalProject)
message("start compile seastar")

set(SEASTAR_ROOT ${CMAKE_BINARY_DIR}/third_party/seastar)
set(SEASTAR_LIB       ${SEASTAR_ROOT}/lib/seastar)
set(SEASTAR_INCLUDE_DIR   ${SEASTAR_ROOT}/include)


list(APPEND BRPC_DEPENDS_PREFIX_PATH ${GFLAGS_ROOT})
# list(APPEND BRPC_DEPENDS_PREFIX_PATH ${THRIFT_INSTALL_DIR})
# list(APPEND BRPC_DEPENDS_PREFIX_PATH ${OPENSSL_INSTALL_DIR})
list(APPEND BRPC_DEPENDS_PREFIX_PATH ${LEVELDB_ROOT})
#list(APPEND BRPC_DEPENDS_PREFIX_PATH ${ZLIB_ROOT})
list(APPEND BRPC_DEPENDS_PREFIX_PATH ${PROTOBUF_ROOT})
list(APPEND BRPC_DEPENDS_PREFIX_PATH "${GLOG_ROOT};")

include_directories(${SEASTAR_INCLUDE_DIR})
link_directories(${SEASTAR_LIB})

set(SEASTAR_GIT_TAG      master)  # 指定版本
set(SEASTAR_GIT_URL      https://github.com/scylladb/seastar.git)
set(SEASTAR_CONFIGURE    cd ${SEASTAR_ROOT}/src/SEASTAR && cmake -DCMAKE_INSTALL_PREFIX=${SEASTAR_ROOT} .)
set(SEASTAR_MAKE         cd ${SEASTAR_ROOT}/src/SEASTAR && make)
set(SEASTAR_INSTALL      cd ${SEASTAR_ROOT}/src/SEASTAR && make install)
#为啥一定是大写呢
ExternalProject_Add(SEASTAR
        PREFIX            ${SEASTAR_ROOT}
        GIT_REPOSITORY    ${SEASTAR_GIT_URL}
        GIT_TAG           ${SEASTAR_GIT_TAG}
        CONFIGURE_COMMAND ${SEASTAR_CONFIGURE}
        BUILD_COMMAND     ${SEASTAR_MAKE}
        INSTALL_COMMAND   ${SEASTAR_INSTALL}
)
message("finish compile seastar")