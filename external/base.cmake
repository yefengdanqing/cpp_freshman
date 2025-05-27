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

include(${PROJECT_SOURCE_DIR}/external/boost/config.cmake)
set(EXTERNAL_PREFIX ${CMAKE_BINARY_DIR}/external)
include(${PROJECT_SOURCE_DIR}/external/protobuf/config.cmake)
include(${PROJECT_SOURCE_DIR}/external/googletest/config.cmake)
include(${PROJECT_SOURCE_DIR}/external/yaml-cpp/config.cmake)

#include(${PROJECT_SOURCE_DIR}/external/fmt/config.cmake)
include(${PROJECT_SOURCE_DIR}/external/absl/config.cmake)

include(${PROJECT_SOURCE_DIR}/external/spdlog/config.cmake)
include(${PROJECT_SOURCE_DIR}/external/leveldb/config.cmake)
include(${PROJECT_SOURCE_DIR}/external/openssl/config.cmake)
include(${PROJECT_SOURCE_DIR}/external/gflags/config.cmake)
#include(${PROJECT_SOURCE_DIR}/external/glog/config.cmake)
include(${PROJECT_SOURCE_DIR}/external/babylon/config.cmake)
#include(${PROJECT_SOURCE_DIR}/external/gperftools/config.cmake)

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


function(add_git_external name)
    # 参数：
    #   name - 库的名字，同时作为 PREFIX 和 TARGET 名称基础
    # 可选变量（必须在调用前用 set() 赋值）：
    #   <name>_GIT_URL       - Git 仓库地址（必填）
    #   <name>_GIT_TAG       - Git 标签或分支（必填）
    #   <name>_INSTALL_DIR   - 安装目录（必填）
    #   <name>_CMAKE_ARGS   - 传给被构建项目的 CMake 参数（可选）
    #   <name>_LIB_PATTERN  - 库文件匹配模式，用于自动收集 so/dll 文件（可选，默认 lib<name>_*.so）

    cmake_parse_arguments(PARSE_ARGV 1
        "" "" ""
    )

    # 取变量值
    set(url_var "${name}_GIT_URL")
    set(tag_var "${name}_GIT_TAG")
    set(install_dir_var "${name}_INSTALL_DIR")
    set(cmake_args_var "${name}_CMAKE_ARGS")
    set(lib_pattern_var "${name}_LIB_PATTERN")

    if(NOT DEFINED ${url_var})
        message(FATAL_ERROR "Variable ${url_var} must be set before calling add_git_external")
    endif()
    if(NOT DEFINED ${tag_var})
        message(FATAL_ERROR "Variable ${tag_var} must be set before calling add_git_external")
    endif()
    if(NOT DEFINED ${install_dir_var})
        message(FATAL_ERROR "Variable ${install_dir_var} must be set before calling add_git_external")
    endif()

    set(url ${${url_var}})
    set(tag ${${tag_var}})
    set(install_dir ${${install_dir_var}})
    set(cmake_args ${${cmake_args_var}})
    set(lib_pattern ${${lib_pattern_var}})

    if(NOT lib_pattern)
        # 默认库文件匹配模式
        string(TOLOWER ${name} lname)
        set(lib_pattern "${install_dir}/lib/lib${lname}_*.so")
    endif()

    # 目录变量
    set(prefix_dir ${install_dir})

    # 避免重复添加 CMAKE_PREFIX_PATH
    list(FIND CMAKE_PREFIX_PATH ${prefix_dir} _idx)
    if(_idx EQUAL -1)
        list(APPEND CMAKE_PREFIX_PATH ${prefix_dir})
    endif()

    ExternalProject_Add(${name}_external
        PREFIX          ${prefix_dir}
        GIT_REPOSITORY  ${url}
        GIT_TAG         ${tag}
        INSTALL_DIR     ${install_dir}
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${install_dir}
            -DCMAKE_INSTALL_LIBDIR=lib
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            -DBUILD_SHARED_LIBS=ON
            -DABSL_BUILD_TESTING=OFF
            ${cmake_args}
    )

    # 收集库文件列表
    file(GLOB_RECURSE libs LIST_DIRECTORIES false "${lib_pattern}")

    add_library(${name} SHARED IMPORTED GLOBAL)
    set_property(TARGET ${name} PROPERTY IMPORTED_LOCATION "${libs}")
    add_dependencies(${name} ${name}_external)

    # 头文件和库路径
    set(${name}_INCLUDE_DIR ${install_dir}/include PARENT_SCOPE)
    set(${name}_LIB_DIR ${install_dir}/lib PARENT_SCOPE)

    include_directories(${${name}_INCLUDE_DIR})
    link_directories(${${name}_LIB_DIR})

    # 把库和依赖加入全局变量，方便后续统一链接
    set(LIB_BIBRARY ${libs} ${LIB_BIBRARY} PARENT_SCOPE)
    set(LIB_DEPENDS ${name} ${LIB_DEPENDS} PARENT_SCOPE)
endfunction()







