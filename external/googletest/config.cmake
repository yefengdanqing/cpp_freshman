include(ExternalProject)
set(THIRD_PARTY_PREFIX ${CMAKE_BINARY_DIR}/third_party)
set(GTEST_ROOT ${THIRD_PARTY_PREFIX}/googletest)
set(GTEST_LIB       ${GTEST_ROOT}/lib)
set(GTEST_INCLUDE_DIR   ${GTEST_ROOT}/include)
set(GTEST_LIB_LIBRARIES "${GTEST_LIB}/libgtest_main.so"
                        "${GTEST_LIB}/libgtest.so"
CACHE FILEPATH "CASSANDRA_DRIVER_LIBRARIES" FORCE)


list(FIND CMAKE_PREFIX_PATH ${GTEST_ROOT} INDEX)
if(INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${GTEST_ROOT})
endif()


find_package(googletest QUIET)
if (NOT googletest_FOUND)  
        ExternalProject_Add(GOOGLETEST
                PREFIX                  ${GTEST_ROOT}
                GIT_REPOSITORY          https://github.com/google/googletest.git
                GIT_TAG                 main
                CONFIGURE_COMMAND       cd ${GTEST_ROOT}/src/GOOGLETEST && cmake 
                                -D CMAKE_INSTALL_PREFIX=${GTEST_ROOT} -DBUILD_SHARED_LIBS=ON 
                                -DCMAKE_INSTALL_LIBDIR=lib .
                BUILD_COMMAND           cd ${GTEST_ROOT}/src/GOOGLETEST && make -j8
                INSTALL_COMMAND         cd ${GTEST_ROOT}/src/GOOGLETEST && make install
        )

endif()

include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIB})
ADD_LIBRARY(googletest STATIC IMPORTED GLOBAL)
SET_PROPERTY(TARGET googletest PROPERTY IMPORTED_LOCATION ${GTEST_LIB_LIBRARIES})
add_dependencies(googletest GOOGLETEST)
set(LIB_BIBRARY
        ${GTEST_LIB_LIBRARIES}
        ${LIB_BIBRARY})
set(LIB_DEPENDS
        "googletest"
        ${LIB_DEPENDS})



