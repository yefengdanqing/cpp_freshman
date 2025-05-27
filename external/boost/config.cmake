include(ExternalProject)
set(EXTERNAL_BOOST_ROOT ${CMAKE_BINARY_DIR}/external/boost)
set(BOOST_GIT_TAG boost-1.83.0)
set(BOOST_GIT_URL https://github.com/boostorg/boost.git)


set(BOOST_LIB_DIR       ${EXTERNAL_BOOST_ROOT}/lib)
set(BOOST_INCLUDE_DIR   ${EXTERNAL_BOOST_ROOT}/include)
set(BOOST_LIBRARIES     "${BOOST_LIB_DIR}/libboost_atomic.a"
                        "${BOOST_LIB_DIR}/libboost_chrono.a"
                        "${BOOST_LIB_DIR}/libboost_container.a"
                        "${BOOST_LIB_DIR}/libboost_context.a"
                        "${BOOST_LIB_DIR}/libboost_contract.a"
                        "${BOOST_LIB_DIR}/libboost_coroutine.a"
                        "${BOOST_LIB_DIR}/libboost_date_time.a"
                        "${BOOST_LIB_DIR}/libboost_exception.a"
                        "${BOOST_LIB_DIR}/libboost_fiber.a"
                        "${BOOST_LIB_DIR}/libboost_fiber_numa.a"
                        "${BOOST_LIB_DIR}/libboost_filesystem.a"
                        "${BOOST_LIB_DIR}/libboost_graph.a"
                        "${BOOST_LIB_DIR}/libboost_iostreams.a"
                        "${BOOST_LIB_DIR}/libboost_json.a"
                        "${BOOST_LIB_DIR}/libboost_locale.a"
                        "${BOOST_LIB_DIR}/libboost_log.a"
                        "${BOOST_LIB_DIR}/libboost_log_setup.a"
                        "${BOOST_LIB_DIR}/libboost_nowide.a"
                        "${BOOST_LIB_DIR}/libboost_prg_exec_monitor.a"
                        "${BOOST_LIB_DIR}/libboost_program_options.a"
                        "${BOOST_LIB_DIR}/libboost_random.a"
                        "${BOOST_LIB_DIR}/libboost_serialization.a"
                        "${BOOST_LIB_DIR}/libboost_stacktrace_addr2line.a"
                        "${BOOST_LIB_DIR}/libboost_stacktrace_basic.a"
                        "${BOOST_LIB_DIR}/libboost_stacktrace_noop.a"
                        "${BOOST_LIB_DIR}/libboost_test_exec_monitor.a"
                        "${BOOST_LIB_DIR}/libboost_thread.a"
                        "${BOOST_LIB_DIR}/libboost_timer.a"
                        "${BOOST_LIB_DIR}/libboost_type_erasure.a"
                        "${BOOST_LIB_DIR}/libboost_unit_test_framework.a"
                        "${BOOST_LIB_DIR}/libboost_url.a"
                        "${BOOST_LIB_DIR}/libboost_wave.a"
                        "${BOOST_LIB_DIR}/libboost_wserialization.a"
CACHE FILEPATH "BOOST_LIBRARIES" FORCE)
set(BOOST_VERSION "boost")
include_directories(${BOOST_INCLUDE_DIR})
link_directories(${BOOST_LIB_DIR})

list(FIND CMAKE_PREFIX_PATH ${EXTERNAL_BOOST_ROOT} _DEP_INDEX)
if (_DEP_INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${EXTERNAL_BOOST_ROOT})
endif ()
#string(REPLACE ";" "|" TBOOST_CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}")

#find_package(boost QUIET)

ExternalProject_Add(Boost
        GIT_REPOSITORY        ${BOOST_GIT_URL}
        GIT_TAG               ${BOOST_GIT_TAG}
        # PREFIX                  "${EXTERNAL_BOOST_ROOT}" #debug code
        SOURCE_DIR            ${EXTERNAL_BOOST_ROOT}
        INSTALL_DIR           ${EXTERNAL_BOOST_ROOT}
        UPDATE_COMMAND ""
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_BOOST_ROOT}
                -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
                -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
                -DCMAKE_INSTALL_LIBDIR=${EXTERNAL_BOOST_ROOT}/lib
                -DCMAKE_PREFIX_PATH=${TBOOST_CMAKE_PREFIX_PATH}
                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                -DBUILD_SHARED_LIBS=OFF
                -DCMAKE_INSTALL_MANIFEST_CONTENT=${EXTERNAL_BOOST_ROOT}
        LIST_SEPARATOR  |
        BUILD_BYPRODUCTS ${BOOST_LIBRARIES})


ADD_LIBRARY(boost STATIC IMPORTED GLOBAL)
SET_PROPERTY(TARGET boost PROPERTY IMPORTED_LOCATION ${BOOST_LIBRARIES})
add_dependencies(boost Boost)
set(LIB_BIBRARY
    ${BOOST_LIBRARIES}
    ${LIB_BIBRARY})

set(LIB_DEPENDS
        "boost"
        ${LIB_DEPENDS})

    
# https://github.com/PaddlePaddle/Paddle/blob/develop/cmake/external/boost.cmake
#https://github.com/tushushu/bigflow/tree/ab494e49a02b446bb2f504a2652f866c924c1baf/cmake

