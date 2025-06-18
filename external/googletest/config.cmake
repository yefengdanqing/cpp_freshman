include(ExternalProject)
set(EXTERNAL_PREFIX ${CMAKE_BINARY_DIR}/external)
set(EXTERNAL_GTEST_ROOT ${EXTERNAL_PREFIX}/googletest)
set(GTEST_LIB       ${EXTERNAL_GTEST_ROOT}/lib)
set(GTEST_INCLUDE_DIR   ${EXTERNAL_GTEST_ROOT}/include)
set(GTEST_LIB_LIBRARIES "${GTEST_LIB}/libgtest_main.so"
                        "${GTEST_LIB}/libgtest.so"
CACHE FILEPATH "GOOLGETEST_LIBRARIES" FORCE)



message("xxxxgoogletest${EXTERNAL_GTEST_ROOT}")
list(FIND CMAKE_PREFIX_PATH ${EXTERNAL_GTEST_ROOT} INDEX)
if(INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${EXTERNAL_GTEST_ROOT})
endif()
        ExternalProject_Add(GOOGLETEST
                PREFIX ${EXTERNAL_GTEST_ROOT}
                GIT_REPOSITORY        git@github.com:google/googletest.git
                GIT_TAG main
                INSTALL_DIR           ${EXTERNAL_GTEST_ROOT}
                CMAKE_ARGS
                        -DCMAKE_INSTALL_PREFIX=${EXTERNAL_GTEST_ROOT}
                        -DBUILD_SHARED_LIBS=ON
                        -DCMAKE_INSTALL_LIBDIR=lib
                        -DBENCHMARK_ENABLE_TESTING=OFF
                        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
                        -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
                        #-DCMAKE_INSTALL_COMPONENT= ${EXTERNAL_GTEST_ROOT}
                LOG_CONFIGURE 1
                LOG_BUILD 1
                LOG_INSTALL 1
        )

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




