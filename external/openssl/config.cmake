#include(${CMAKE_CURRENT_LIST_DIR}/../gflags/config.cmake)
#include(${PROJECT_SOURCE_DIR}/external/googletest/config.cmake)
# find_package(Glog)
# if (OPENSSL_FOUND)
#     set(OPENSSL_EXTERNAL FALSE)
# endif()


include(ExternalProject)

set(EXTERNAL_OPENSSL_ROOT ${CMAKE_BINARY_DIR}/external/openssl)
set(OPENSSL_ROOT_DIR ${EXTERNAL_OPENSSL_ROOT})

#指定版本
set(OPENSSL_GIT_URL git@github.com:openssl/openssl.git)  # 指定git仓库地址

# 指定编译好的静态库文件的路径
set(OPENSSL_LIB_DIR       ${EXTERNAL_OPENSSL_ROOT}/lib64)
# 指定头文件所在的目录
set(OPENSSL_INCLUDE_DIR   ${EXTERNAL_OPENSSL_ROOT}/include)
set(OPENSSL_LIBRARIES "${OPENSSL_LIB_DIR}/libssl.a"
                        "${OPENSSL_LIB_DIR}/libcrypto.a" CACHE FILEPATH "SSL_LIBRARIES" FORCE)

set(OPENSSL_CRYPTO_LIBRARY  "${OPENSSL_LIB_DIR}/libcrypto.a")
set(OPENSSL_SSL_LIBRARY  "${OPENSSL_LIB_DIR}/libssl.a")

include_directories(${OPENSSL_INCLUDE_DIR})
link_directories(${OPENSSL_LIB_DIR})

set(OPENSSL_CONFIGURE    cd ${EXTERNAL_OPENSSL_ROOT}/src/OPENSSL && ./Configure --prefix=${EXTERNAL_OPENSSL_ROOT} --openssldir=${EXTERNAL_OPENSSL_ROOT} -Wl,-rpath,$(LIBRPATH))
#./config no-shared no-idea -fPIC --prefix=${OPENSSL_INSTALL_DIR}

set(OPENSSL_MAKE         cd ${EXTERNAL_OPENSSL_ROOT}/src/OPENSSL && make depend && make -j8)  # 指定编译指令（需要覆盖默认指令，进入我们指定的OPENSSL_ROOT目录下）
set(OPENSSL_INSTALL      cd ${EXTERNAL_OPENSSL_ROOT}/src/OPENSSL && make install)  # 指定安装指令（需要覆盖默认指令，进入我们指定的OPENSSL_ROOT目录下,可以copy 出来
 # 如果不存在，则需要编译,要先创建目标
list(FIND CMAKE_PREFIX_PATH ${EXTERNAL_OPENSSL_ROOT} _DEP_INDEX)
if (_DEP_INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${EXTERNAL_OPENSSL_ROOT})
endif ()

find_package(OpenSSL QUIET)


ExternalProject_Add(OPENSSL
                PREFIX            ${EXTERNAL_OPENSSL_ROOT}          
                GIT_REPOSITORY    ${OPENSSL_GIT_URL}
                GIT_TAG           ${OPENSSL_GIT_TAG}
                CONFIGURE_COMMAND ${OPENSSL_CONFIGURE}
                BUILD_COMMAND     ${OPENSSL_MAKE}
                INSTALL_COMMAND   ${OPENSSL_INSTALL}
                # LOG_CONFIGURE     1
                # LOG_INSTALL       1
                CMAKE_ARGS      -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                                -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                                -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
                                -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
                                -DCMAKE_PREFIX_PATH=${OPENSSL_SOURCES_DIR}
                                -DCMAKE_INSTALL_PREFIX=${EXTERNAL_OPENSSL_ROOT/}
                                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
		                -DBUILD_SHARED_LIBS=OFF
                                -DBUILD_STATIC_LIBS=ON
                                -DWITH_GFLAGS=ON
                                -DBUILD_GMOCK=OFF
                                -DCMAKE_INSTALL_LIBDIR=lib
        )


message("skt lib64: ${OPENSSL_LIB_DIR}")
        include_directories(${OPENSSL_INCLUDE_DIR})
        link_directories(${OPENSSL_LIB_DIR})



add_library(openssl_openssl STATIC IMPORTED GLOBAL)
set_property(TARGET openssl_openssl PROPERTY IMPORTED_LOCATION ${OPENSSL_LIB_DIR}/libssl.so)
add_dependencies(openssl_openssl OPENSSL)
set(LIB_BIBRARY
    ${OPENSSL_LIBRARIES}
    ${LIB_BIBRARY})
set(LIB_DEPENDS
        ${LIB_DEPENDS}
        "openssl_openssl")



# add_library(openssl gflags)

