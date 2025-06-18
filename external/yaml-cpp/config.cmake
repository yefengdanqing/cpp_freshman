# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(ExternalProject)

# 定义外部库的根目录
set(EXTERNAL_YAMLCPP_ROOT ${EXTERNAL_PREFIX}/yaml-cpp)
set(YAMLCPP_LIB_DIR ${EXTERNAL_YAMLCPP_ROOT}/lib)
set(YAMLCPP_INCLUDE_DIR ${EXTERNAL_YAMLCPP_ROOT}/include)

# 设置静态库路径（注意这里修改为 yaml-cpp 库的实际路径）
set(YAMLCPP_LIB_LIBRARIES "${YAMLCPP_LIB_DIR}/libyaml-cpp.a"
    CACHE FILEPATH "YAMLCPP_LIBRARIES" FORCE)

# 定义 Git 标签
set(YAMLCPP_GIT_TAG master)

# 设置 Git 仓库 URL
set(YAMLCPP_GIT_URL git@github.com:jbeder/yaml-cpp.git)

# 将 yaml-cpp 的 include 目录添加到 CMake 的包含路径中
include_directories(${YAMLCPP_INCLUDE_DIR})

# 将 yaml-cpp 的 lib 目录添加到 CMake 的链接路径中
link_directories(${YAMLCPP_LIB_DIR})

# 如果 CMake prefix path 中没有包含 yaml-cpp 根目录，添加它
list(FIND CMAKE_PREFIX_PATH ${EXTERNAL_YAMLCPP_ROOT} _DEP_INDEX)
if (_DEP_INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${EXTERNAL_YAMLCPP_ROOT})
endif ()
string(REPLACE ";" "|" TYAMLCPP_CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}")

# 检查 yaml-cpp 是否已经找到
#find_package(yaml-cpp QUIET)
# if (NOT yamlcpp_FOUND)
    ExternalProject_Add(YAMLCPP
        GIT_REPOSITORY ${YAMLCPP_GIT_URL}
        GIT_TAG ${YAMLCPP_GIT_TAG}
        SOURCE_DIR ${EXTERNAL_YAMLCPP_ROOT}
        # DEPENDS Boost protobuf_protobuf absl
        INSTALL_DIR ${EXTERNAL_YAMLCPP_ROOT}
        UPDATE_COMMAND ""
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_YAMLCPP_ROOT}
                   -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                   -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                   -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
                   -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
                   -DCMAKE_POSITION_INDEPENDENT_CODE=ON
                   -DCMAKE_INSTALL_LIBDIR=${YAMLCPP_LIB_DIR}
                   -DCMAKE_PREFIX_PATH=${TYAMLCPP_CMAKE_PREFIX_PATH}
                   -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                   -DBUILD_SHARED_LIBS=OFF
                   -DBUILD_TESTING=OFF
                   -DCMAKE_CXX_STANDARD=17
                   -DCMAKE_CXX_STANDARD_REQUIRED=ON
        LIST_SEPARATOR |
        BUILD_BYPRODUCTS ${YAMLCPP_LIB_LIBRARIES})
# endif()

# 导入 yaml-cpp 静态库
ADD_LIBRARY(yaml-cpp STATIC IMPORTED GLOBAL)
SET_PROPERTY(TARGET yaml-cpp PROPERTY IMPORTED_LOCATION ${YAMLCPP_LIB_LIBRARIES})

# 添加依赖项
add_dependencies(yaml-cpp YAMLCPP ${LIB_DEPENDS})

# 更新 LIB_BIBRARY 变量
set(LIB_BIBRARY
    ${YAMLCPP_LIB_LIBRARIES}
    ${LIB_BIBRARY})

# 更新 LIB_DEPENDS 变量
set(LIB_DEPENDS
    "yaml-cpp"
    ${LIB_DEPENDS})



# include(FetchContent)

# FetchContent_Declare(
#   yaml-cpp
#   GIT_REPOSITORY git@github.com:jbeder/yaml-cpp.git
#   GIT_TAG master # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
# )
# FetchContent_MakeAvailable(yaml-cpp)

# target_link_libraries(yaml-cpp PUBLIC yaml-cpp::yaml-cpp) # The library or executable that require yaml-cpp library

# # 更新 LIB_DEPENDS 变量
# set(LIB_DEPENDS
#     "yaml-cpp"
#     ${LIB_DEPENDS})