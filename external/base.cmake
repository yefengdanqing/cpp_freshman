find_program(MAKE_EXECUTABLE NAMES make gmake mingw32-make REQUIRED)

# function(get_cmake_args)
#     #string(REPLACE ";" "|" CMAKE_PREFIX_PATH_STR "${CMAKE_PREFIX_PATH}")
#     #string(REPLACE ";" "\\;" CMAKE_PREFIX_PATH_STR "${CMAKE_PREFIX_PATH}")
#     set(CMAKE_ARGS
#             #-DCMAKE_PREFIX_PATH=$<CMAKE_PREFIX_PATH>
#             -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
#             -DCMAKE_BUILD_TYPE=Release
#             -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}/${DEP_NAME}
#             -DCMAKE_INSTALL_LIBDIR=lib
#             -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#             -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#             -DBUILD_STATIC_LIB=OFF
#             -DBUILD_SHARED_LIB=ON PARENT_SCOPE)
# endfunction(get_cmake_args)






# 建议使用压缩包的方式依赖，下载速度更快
function(thirdparty_in_fetchcontent)
    message("thirdparty_in_fetchcontent")
endfunction(thirdparty_in_fetchcontent)

#不要用master,除非人为升级
#begin include
set(EXTERNAL_PREFIX ${CMAKE_BINARY_DIR}/external)
include(${PROJECT_SOURCE_DIR}/external/protobuf/config.cmake)

include(${PROJECT_SOURCE_DIR}/external/boost/config.cmake)
#include(${PROJECT_SOURCE_DIR}/external/fmt/config.cmake)
include(${PROJECT_SOURCE_DIR}/external/absl/config.cmake)

include(${PROJECT_SOURCE_DIR}/external/spdlog/config.cmake)
include(${PROJECT_SOURCE_DIR}/external/leveldb/config.cmake)
include(${PROJECT_SOURCE_DIR}/external/openssl/config.cmake)
include(${PROJECT_SOURCE_DIR}/external/gflags/config.cmake)
# include(${PROJECT_SOURCE_DIR}/external/glog/config.cmake)
include(${PROJECT_SOURCE_DIR}/external/googletest/config.cmake)
include(${PROJECT_SOURCE_DIR}/external/gperftools/config.cmake)



#include(${PROJECT_SOURCE_DIR}/external/libuv/config.cmake)
#include(${PROJECT_SOURCE_DIR}/external/cassandra_driver/config.cmake)
include(${PROJECT_SOURCE_DIR}/external/brpc/config.cmake)

#end include

#generate pb.h
function(CUSTOM_PROTOBUF_GENERATE_CPP SRCS HDRS)
    if(NOT ARGN)
        message(SEND_ERROR "Error: CUSTOM_PROTOBUF_GENERATE_CPP() called without any proto files")
        return()
    endif()
  # Create an include path for each file specified
    foreach(FIL ${ARGN})
        message("custom generate ${FIL}")
        get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
        get_filename_component(ABS_PATH ${ABS_FIL} PATH)
        list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
        if(${_contains_already} EQUAL -1)
            list(APPEND _protobuf_include_path -I ${ABS_PATH})
        endif()
    endforeach()

    set(${SRCS})
    set(${HDRS})
    message("before " ${SRCS} ${${SRCS}})
    foreach(FIL ${ARGN})
        message("CUSTOM_PROTOBUF_GENERATE_CPP ${FIL}")
        get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
        get_filename_component(FIL_WE ${FIL} NAME_WE)

        

        #execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR})
        set(dest_proto_path ${PROJECT_SOURCE_DIR}/proto)
        list(APPEND ${SRCS} "${protocol_protobuf}/${FIL_WE}.pb.cc")
        list(APPEND ${HDRS} "${protocol_protobuf}/${FIL_WE}.pb.h")


        message("generate protocolbuf file info:[${CMAKE_CURRENT_BINARY_DIR}][${protocol_protobuf}/${FIL_WE}.pb.cc][${PBUF_PROTOC}][${_protobuf_include_path}][${ABS_FIL}][${protocol_protobuf}][${PBUF_PROTOC}]")

        add_custom_command(
            OUTPUT "${protocol_protobuf}/${FIL_WE}.pb.cc"
             "${protocol_protobuf}/${FIL_WE}.pb.h"
            COMMAND ${PBUF_PROTOC} -I ${PROJECT_SOURCE_DIR}/proto --cpp_out ${protocol_protobuf} ${ABS_FIL}
            DEPENDS ${ABS_FIL}
            COMMENT "skt Running C++ protocol buffer compiler on ${FIL}"
            VERBATIM)
    endforeach()
    message("result xxxxx:" ${SRCS} ${${SRCS}})
    set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
    set(${SRCS} ${${SRCS}} PARENT_SCOPE)
    set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()

#协议相关的操作
set(protocol_protobuf ${PROJECT_BINARY_DIR}/protocol/protobuf/cpp)
file(MAKE_DIRECTORY ${protocol_protobuf})
include_directories(${PROJECT_BINARY_DIR}/protocol/protobuf/cpp)






