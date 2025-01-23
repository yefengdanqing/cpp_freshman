set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(ExternalProject)
set(EXTERNAL_BABYLON_ROOT ${EXTERNAL_PREFIX}/babylon)
set(BABYLON_LIB_DIR       ${EXTERNAL_BABYLON_ROOT}/lib)
set(BABYLON_INCLUDE_DIR   ${EXTERNAL_BABYLON_ROOT}/include)
set(BABYLON_LIB_LIBRARIES "${BABYLON_LIB_DIR}/libbabylon.a"
CACHE FILEPATH "BABYLON_LIBRARIES" FORCE)
set(BABYLON_GIT_TAG main)
#set(BABYLON_GIT_URL https://github.com/yefengdanqing/babylon_cpp.git)
set(BABYLON_GIT_URL https://github.com/baidu/babylon.git)


include_directories(${BABYLON_INCLUDE_DIR})
link_directories(${BABYLON_LIB_DIR})

list(FIND CMAKE_PREFIX_PATH ${EXTERNAL_BABYLON_ROOT} _DEP_INDEX)
if (_DEP_INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${EXTERNAL_BABYLON_ROOT})
endif ()
string(REPLACE ";" "|" TBABYLON_CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}")



#find_package(babylon QUIET)
#if (NOT babylon_FOUND)
    ExternalProject_Add(BABYLON
        GIT_REPOSITORY        ${BABYLON_GIT_URL}
        GIT_TAG               ${BABYLON_GIT_TAG}
        # PREFIX                  "${EXTERNAL_BABYLON_ROOT}" #debug code
        SOURCE_DIR            "${EXTERNAL_BABYLON_ROOT}"
        DEPENDS               Boost protobuf_protobuf absl
        INSTALL_DIR           "${EXTERNAL_BABYLON_ROOT}"
        UPDATE_COMMAND ""
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_BABYLON_ROOT}
                -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
                -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
                -DCMAKE_INSTALL_LIBDIR=${BABYLON_LIB_DIR}
                -DCMAKE_PREFIX_PATH=${TBABYLON_CMAKE_PREFIX_PATH}
                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                -DBUILD_SHARED_LIBS=OFF
                -DBUILD_TESTING=OFF
                -DCMAKE_CXX_STANDARD=17
                -DCMAKE_CXX_STANDARD_REQUIRED=ON
        LIST_SEPARATOR  |
        BUILD_BYPRODUCTS ${BABYLON_LIB_LIBRARIES})
#endif()


ADD_LIBRARY(babylon STATIC IMPORTED GLOBAL)
SET_PROPERTY(TARGET babylon PROPERTY IMPORTED_LOCATION ${BABYLON_LIB_LIBRARIES})
add_dependencies(babylon BABYLON ${LIB_DEPENDS})
set(LIB_BIBRARY
    ${BABYLON_LIB_LIBRARIES}
    ${LIB_BIBRARY})

set(LIB_DEPENDS
        "babylon"
        ${LIB_DEPENDS})

    
# https://github.com/PaddlePaddle/Paddle/blob/develop/cmake/external/babylon.cmake
#https://github.com/tushushu/bigflow/tree/ab494e49a02b446bb2f504a2652f866c924c1baf/cmake

