INCLUDE(ExternalProject)

set(RAPIDJSON_INSTALL_DIR "${PROJECT_BINARY_DIR}/third_party/rapidjson")
set(RAPIDJSON_INCLUDE_DIR "${RAPIDJSON_INSTALL_DIR}/include" CACHE PATH "rapidjson include directory." FORCE)
set(RAPIDJSON_LIBRARIES   "${RAPIDJSON_INSTALL_DIR}/lib/librapidjson.a" CACHE FILEPATH "RAPIDJSON_LIBRARIES" FORCE)

include_directories(SYSTEM ${RAPIDJSON_INCLUDE_DIR})

set(RAPIDJSON_VERSION "rapidjson-1.1.0")
execute_process(
    COMMAND tar -xvzf ${PROJ_THIRD_PARTY_DIR}/rapidjson/${RAPIDJSON_VERSION}.tar.gz -C ${PROJ_THIRD_PARTY_UNZIP_DIR}
    RESULT_VARIABLE RAPIDJSON_SOURCE_UNZIP_SUCCESS
)

if(NOT "${RAPIDJSON_SOURCE_UNZIP_SUCCESS}" STREQUAL "0")
    message(FATAL_ERROR "unzip && patch tar failed -> ${RAPIDJSON_SOURCE_UNZIP_SUCCESS}")
endif()

set(RAPIDJSON_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=class-memaccess -Wno-error=implicit-fallthrough")

include_directories(${RAPIDJSON_INSTALL_DIR})
ExternalProject_Add(
    rapidjson_rapidjson
    PREFIX          ${RAPIDJSON_SOURCES_DIR}
    SOURCE_DIR          "${PROJ_THIRD_PARTY_UNZIP_DIR}/${RAPIDJSON_VERSION}"
    BUILD_BYPRODUCTS    ${RAPIDJSON_VERSION}
    UPDATE_COMMAND   ""
    CMAKE_ARGS      -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                    -DCMAKE_CXX_FLAGS=${RAPIDJSON_CMAKE_CXX_FLAGS}
                    -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
                    -DCMAKE_PREFIX_PATH=${RAPIDJSON_SOURCES_DIR}
                    -DCMAKE_INSTALL_LIBDIR=lib
                    -DCMAKE_INSTALL_PREFIX=${RAPIDJSON_INSTALL_DIR}
                    -DCMAKE_POSITION_INDEPENDENT_CODE=ON
                    -DBUILD_SHARED_LIBS=OFF
                    -DBUILD_STATIC_LIBS=ON
                    -DRAPIDJSON_BUILD_EXAMPLES=OFF
                    -DRAPIDJSON_BUILD_TESTS=OFF
)

message("RAPIDJSON_LIBRARIES = ${RAPIDJSON_LIBRARIES}")
add_library(rapidjson STATIC IMPORTED GLOBAL)
set_property(TARGET rapidjson PROPERTY IMPORTED_LOCATION ${RAPIDJSON_LIBRARIES})
add_dependencies(rapidjson rapidjson_rapidjson)

set(LIB_BIBRARY
    ${LIB_BIBRARY}
    ${RAPIDJSON_LIBRARIES})
set(LIB_DEPENDS
        ${BRPC_LIBRARIES}
        "rapidjson")
