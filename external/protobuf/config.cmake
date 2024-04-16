# include(${CMAKE_CURRENT_LIST_DIR}/../base.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/../absl/config.cmake)
set(CMAKE_CXX_STANDARD 17)
INCLUDE(ExternalProject)
set(EXTERNAL_PROTOBUF_ROOT ${CMAKE_BINARY_DIR}/external/protobuf)
file(MAKE_DIRECTORY ${EXTERNAL_PROTOBUF_ROOT}/protobuf)
set(PROTOBUF_LIB_DIR       ${EXTERNAL_PROTOBUF_ROOT}/lib)
set(PROTOBUF_INCLUDE_DIR   ${EXTERNAL_PROTOBUF_ROOT}/include)
set(PROTOBUF_BIN_DIR       ${EXTERNAL_PROTOBUF_ROOT}/bin)
set(PROTOBUF_SELF_LIBRARIES "${PROTOBUF_LIB_DIR}/libprotobuf-lite.a"
                        "${PROTOBUF_LIB_DIR}/libprotobuf.a"
                        "${PROTOBUF_LIB_DIR}/libprotoc.a" CACHE FILEPATH "PROTOBUF_SELF_LIBRARIES" FORCE)
set(PBUF_PROTOC            ${PROTOBUF_BIN_DIR}/protoc)
# set(PROTOBUF_LIBRARIES "${PROTOBUF_LIB_DIR}/libprotobuf.a" CACHE PATH "Path to gflags library files")

include_directories(${PROTOBUF_INCLUDE_DIR})
link_directories(${PROTOBUF_LIB_DIR})

set(PROTOBUF_GIT_TAG  v3.21.3)  # 指定版本
set(PROTOBUF_GIT_URL  https://github.com/protocolbuffers/protobuf.git) 
set(PROTOBUF_VERSION "protobuf-3.5.1")

# execute_process(
#     COMMAND tar -xvzf ${PROJECT_SOURCE_DIR}/external/protobuf/${PROTOBUF_VERSION}.tar.gz -C ${EXTERNAL_PROTOBUF_ROOT}
#     RESULT_VARIABLE PROTOBUF_SOURCE_UNZIP_SUCCESS
# )
# if(NOT "${PROTOBUF_SOURCE_UNZIP_SUCCESS}" STREQUAL "0")
#     message(FATAL_ERROR "unzip && patch tar failed -> ${PROTOBUF_SOURCE_UNZIP_SUCCESS}")
# endif()

# list(FIND CMAKE_PREFIX_PATH ${PROTOBUF_LIB_DIR} _DEP_INDEX)
# if (_DEP_INDEX EQUAL -1)
#     list(APPEND CMAKE_PREFIX_PATH ${PROTOBUF_LIB_DIR})
# endif ()
list(FIND CMAKE_PREFIX_PATH ${EXTERNAL_PROTOBUF_ROOT} _DEP_INDEX)
if (_DEP_INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${EXTERNAL_PROTOBUF_ROOT})
endif ()
string(REPLACE ";" "|" TPROTO_CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}")

message("SOURCE_DIR SOURCE_DIR SOURCE_DIR:${SOURCE_DIR}")
ExternalProject_Add(
  PROTOBUF
  GIT_REPOSITORY       ${PROTOBUF_GIT_URL}
  GIT_TAG              ${PROTOBUF_GIT_TAG}
  SOURCE_DIR          "${EXTERNAL_PROTOBUF_ROOT}/${PROTOBUF_VERSION}"
  INSTALL_DIR         "{EXTERNAL_PROTOBUF_ROOT}"
  BUILD_BYPRODUCTS    ${PROTOBUF_SELF_LIBRARIES}
  CONFIGURE_COMMAND   cd <SOURCE_DIR> && ${CMAKE_COMMAND}
                        -Dprotobuf_BUILD_TESTS=OFF
                        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
                        -DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROTOBUF_ROOT}
                        -DCMAKE_PREFIX_PATH=${TPROTO_CMAKE_PREFIX_PATH}
                        -DCMAKE_INSTALL_LIBDIR=lib
                        -DBUILD_SHARED_LIBS=OFF
                        # ./cmake
  UPDATE_COMMAND      ""
  # BUILD_COMMAND       cd <SOURCE_DIR> && ./configure --prefix=${EXTERNAL_PROTOBUF_ROOT} && make check && make -j8 && make install
  BUILD_COMMAND       cd <SOURCE_DIR> && cmake . -DCMAKE_CXX_STANDARD=14 -DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROTOBUF_ROOT} -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_LIBDIR=lib -D_GLIBCXX_USE_CXX11_ABI=0 && cmake --build . --parallel 10 && cmake --install .
  BUILD_IN_SOURCE 1
)


#需要编译成静态库,protoc也需要依赖
add_library(protobuf_protobuf STATIC IMPORTED GLOBAL)
set_property(TARGET protobuf_protobuf PROPERTY IMPORTED_LOCATION ${PROTOBUF_SELF_LIBRARIES})
add_dependencies(protobuf_protobuf PROTOBUF)


set(LIB_BIBRARY
    ${LIB_BIBRARY}
    ${PROTOBUF_SELF_LIBRARIES})
set(LIB_DEPENDS
        ${LIB_DEPENDS}
        "protobuf_protobuf")
