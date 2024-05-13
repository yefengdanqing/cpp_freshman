include(ExternalProject)
set(EXTERNAL_BABYLON_ROOT ${EXTERNAL_PREFIX}/babylon)
set(BABYLON_LIB       ${EXTERNAL_BABYLON_ROOT}/lib)
set(BABYLON_INCLUDE_DIR   ${EXTERNAL_BABYLON_ROOT}/include)
set(BABYLON_LIB_LIBRARIES "${EXTERNAL_BABYLON_ROOT}/src/BABYLON/libbabylon.a"
CACHE FILEPATH "BABYLON_LIBRARIES" FORCE)

#find_library(ABSL_BASE_LIB absl_base)
message("xxxxxxxxxxxxxxxxx${ABSL_BASE_LIB}")
if(NOT TARGET absl::base)
        message("not not not not  find absl::base")
else()
        message("find find absl::base")
endif()
# get_target_property(BASE_LIBS absl::base INTERFACE_LINK_LIBRARIES)
# message("absl::base 链接的库: ${BASE_LIBS}")
# get_target_property(BASE_LOCATION absl::base LOCATION)
# message("absl::base 路径: ${BASE_LOCATION}")
# message("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")


# if(ABSL_BASE_LIB)
#         ADD_LIBRARY(absl::base SHARED IMPORTED)
#         set_target_properties(absl::base PROPERTIES IMPORTED_LOCATION ${ABSL_LIBRARY_DIR}/libabsl_base.so)
#     message("find base")
# else()
#     message(FATAL_ERROR "absl_base library not found")
# endif()

list(FIND CMAKE_PREFIX_PATH ${EXTERNAL_BABYLON_ROOT} INDEX)
if(INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${EXTERNAL_BABYLON_ROOT})
endif()

string(REPLACE ";" "|" BABYLON_CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}")

# find_package(babylon QUIET)
# if (NOT googletest_FOUND)  
        ExternalProject_Add(BABYLON
                PREFIX                  ${EXTERNAL_BABYLON_ROOT}
                GIT_REPOSITORY          https://github.com/yefengdanqing/babylon_cpp.git
                # GIT_TAG                 5-feat-support-cmake
                GIT_TAG                 main
                CONFIGURE_COMMAND       cd ${EXTERNAL_BABYLON_ROOT}/src/BABYLON && cmake 
                                -DCMAKE_INSTALL_PREFIX=${EXTERNAL_BABYLON_ROOT}
                                -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                                -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                                -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
                                -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
                                -DCMAKE_PREFIX_PATH=${BABYLON_CMAKE_PREFIX_PATH}
                                -DBUILD_SHARED_LIBS=OFF
                                -DBUILD_TESTING=ON
                                # -DBUILD_DEPS=ON
                                # -Dabsl_DIR=${ABSL_LIBRARY_DIR}
                                # -DProtobuf_LIBRARIES=${PROTOBUF_LIB_DIR}
                                # -DProtobuf_INCLUDE_DIR=${PROTOBUF_INCLUDE_DIR}
                                # -DBoost_INCLUDE_DIR=${BOOST_ROOT}
                                -DGTest_DIR=${EXTERNAL_GTEST_ROOT}
                                -DCMAKE_INSTALL_LIBDIR=lib .
                BUILD_COMMAND           cd ${EXTERNAL_BABYLON_ROOT}/src/BABYLON && make -j8
                INSTALL_COMMAND         cd ${EXTERNAL_BABYLON_ROOT}/src/BABYLON && make install
        )

# endif()

include_directories(${BABYLON_INCLUDE_DIR})
link_directories(${BABYLON_LIB})
ADD_LIBRARY(babylon STATIC IMPORTED GLOBAL)
SET_PROPERTY(TARGET babylon PROPERTY IMPORTED_LOCATION ${BABYLON_LIB_LIBRARIES})
add_dependencies(babylon BABYLON)

set(LIB_BIBRARY
        ${BABYLON_LIB_LIBRARIES}
        ${LIB_BIBRARY})
set(LIB_DEPENDS
        "babylon"
        ${LIB_DEPENDS})



