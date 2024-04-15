include(ExternalProject)
set(ABSL_PROPAGATE_CXX_STD ON)
set(THIRD_PARTY_PREFIX ${CMAKE_BINARY_DIR}/third_party)
set(ABSL_INSTALL_DIR "${PROJECT_BINARY_DIR}/third_party/absl")
set(ABSL_INCLUDE_DIR "${ABSL_INSTALL_DIR}/include" CACHE PATH "a'b's include directory." FORCE)
set(ABSL_LIBRARY_DIR "${ABSL_INSTALL_DIR}/lib")
set(ABSL_LIBRARIES   "${ABSL_LIBRARY_DIR}/libabsl_bad_any_cast_impl.so"
                "${ABSL_LIBRARY_DIR}/libabsl_strings.so"
                "${ABSL_LIBRARY_DIR}/libabsl_strings_internal.so"
                "${ABSL_LIBRARY_DIR}/libabsl_str_format_internal.so"
                "${ABSL_LIBRARY_DIR}/libabsl_time.so"
                "${ABSL_LIBRARY_DIR}/libabsl_time_zone.so"
                "${ABSL_LIBRARY_DIR}/libabsl_civil_time.so"
                "${ABSL_LIBRARY_DIR}/libabsl_bad_optional_access.so"
                "${ABSL_LIBRARY_DIR}/libabsl_bad_variant_access.so"
                "${ABSL_LIBRARY_DIR}/libabsl_base.so"
                "${ABSL_LIBRARY_DIR}/libabsl_city.so"
                "${ABSL_LIBRARY_DIR}/libabsl_cord.so"
                "${ABSL_LIBRARY_DIR}/libabsl_cord_internal.so"
                "${ABSL_LIBRARY_DIR}/libabsl_cordz_functions.so"
                "${ABSL_LIBRARY_DIR}/libabsl_cordz_handle.so"
                "${ABSL_LIBRARY_DIR}/libabsl_cordz_info.so"
                "${ABSL_LIBRARY_DIR}/libabsl_cordz_sample_token.so"
                "${ABSL_LIBRARY_DIR}/libabsl_debugging_internal.so"
                "${ABSL_LIBRARY_DIR}/libabsl_demangle_internal.so"
                "${ABSL_LIBRARY_DIR}/libabsl_examine_stack.so"
                "${ABSL_LIBRARY_DIR}/libabsl_exponential_biased.so"
                "${ABSL_LIBRARY_DIR}/libabsl_failure_signal_handler.so"
                "${ABSL_LIBRARY_DIR}/libabsl_flags_commandlineflag.so"
                "${ABSL_LIBRARY_DIR}/libabsl_flags_commandlineflag_internal.so"
                "${ABSL_LIBRARY_DIR}/libabsl_flags_config.so"
                "${ABSL_LIBRARY_DIR}/libabsl_flags_internal.so"
                "${ABSL_LIBRARY_DIR}/libabsl_flags_marshalling.so"
                "${ABSL_LIBRARY_DIR}/libabsl_flags_parse.so"
                "${ABSL_LIBRARY_DIR}/libabsl_flags_private_handle_accessor.so"
                "${ABSL_LIBRARY_DIR}/libabsl_flags_program_name.so"
                "${ABSL_LIBRARY_DIR}/libabsl_flags_reflection.so"
                "${ABSL_LIBRARY_DIR}/libabsl_flags_usage.so"
                "${ABSL_LIBRARY_DIR}/libabsl_flags_usage_internal.so"
                "${ABSL_LIBRARY_DIR}/libabsl_graphcycles_internal.so"
                "${ABSL_LIBRARY_DIR}/libabsl_hash.so"
                "${ABSL_LIBRARY_DIR}/libabsl_hashtablez_sampler.so"
                "${ABSL_LIBRARY_DIR}/libabsl_int128.so"
                "${ABSL_LIBRARY_DIR}/libabsl_leak_check.so"
                "${ABSL_LIBRARY_DIR}/libabsl_log_severity.so"
                "${ABSL_LIBRARY_DIR}/libabsl_low_level_hash.so"
                "${ABSL_LIBRARY_DIR}/libabsl_malloc_internal.so"
                "${ABSL_LIBRARY_DIR}/libabsl_periodic_sampler.so"
                "${ABSL_LIBRARY_DIR}/libabsl_random_distributions.so"
                "${ABSL_LIBRARY_DIR}/libabsl_random_internal_distribution_test_util.so"
                "${ABSL_LIBRARY_DIR}/libabsl_random_internal_platform.so"
                "${ABSL_LIBRARY_DIR}/libabsl_random_internal_pool_urbg.so"
                "${ABSL_LIBRARY_DIR}/libabsl_random_internal_randen.so"
                "${ABSL_LIBRARY_DIR}/libabsl_random_internal_randen_hwaes.so"
                "${ABSL_LIBRARY_DIR}/libabsl_random_internal_randen_hwaes_impl.so"
                "${ABSL_LIBRARY_DIR}/libabsl_random_internal_randen_slow.so"
                "${ABSL_LIBRARY_DIR}/libabsl_random_internal_seed_material.so"
                "${ABSL_LIBRARY_DIR}/libabsl_random_seed_gen_exception.so"
                "${ABSL_LIBRARY_DIR}/libabsl_random_seed_sequences.so"
                "${ABSL_LIBRARY_DIR}/libabsl_raw_hash_set.so"
                "${ABSL_LIBRARY_DIR}/libabsl_raw_logging_internal.so"
                "${ABSL_LIBRARY_DIR}/libabsl_scoped_set_env.so"
                "${ABSL_LIBRARY_DIR}/libabsl_spinlock_wait.so"
                "${ABSL_LIBRARY_DIR}/libabsl_stacktrace.so"
                "${ABSL_LIBRARY_DIR}/libabsl_status.so"
                "${ABSL_LIBRARY_DIR}/libabsl_statusor.so"
                "${ABSL_LIBRARY_DIR}/libabsl_strerror.so"
                "${ABSL_LIBRARY_DIR}/libabsl_symbolize.so"
                "${ABSL_LIBRARY_DIR}/libabsl_synchronization.so"
                "${ABSL_LIBRARY_DIR}/libabsl_throw_delegate.so"
 CACHE FILEPATH "ABSL_LIBRARIES" FORCE)

set(ABSL_ROOT ${THIRD_PARTY_PREFIX}/absl)
set(ABSL_GIT_TAG  master)  # 指定版本
set(ABSL_GIT_URL https://github.com/abseil/abseil-cpp.git)  # 指定git仓库地址


set(ABSL_CONFIGURE    cd ${ABSL_ROOT}/src/ABSL && rm -rf build && mkdir build && cd build && CXXFLAGS=-fPIC cmake .. -DCMAKE_INSTALL_PREFIX=${ABSL_ROOT} -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_LIBDIR=lib -DABSL_BUILD_TESTING=OFF -DABSL_USE_GOOGLETEST_HEAD=OFF)  # 指定配置指令（注意此处修改了安装目录，否则默认情况下回安装到系统目录)

set(ABSL_MAKE         cd ${ABSL_ROOT}/src/ABSL/build && CC=gcc CXX=g++ CXXFLAGS=-fPIC make -j8)  # 指定编译指令（需要覆盖默认指令，进入我们指定的ABSL_ROOT目录下)

set(ABSL_INSTALL      cd ${ABSL_ROOT}/src/ABSL/build && make install)  # 指定安装指令（需要覆盖默认指令，进入我们指定的ABSL_ROOT目录下,可以copy 出来


# 指定编译好的静态库文件的路径
set(ABSL_LIB_DIR       ${ABSL_ROOT}/lib)
# 指定头文件所在的目录
set(ABSL_INCLUDE_DIR   ${ABSL_ROOT}/include)

message("add depends dirs: [${ABSL_LIB_DIR}][${ABSL_INCLUDE_DIR}]")
link_directories(${ABSL_LIB_DIR})
include_directories(${ABSL_INCLUDE_DIR})

message("cmake_module_path: " ${CMAKE_MODULE_PATH})


list(FIND CMAKE_PREFIX_PATH ${ABSL_ROOT} INDEX)
if(INDEX EQUAL -1)
    list(APPEND CMAKE_PREFIX_PATH ${ABSL_ROOT})
endif()
string (REPLACE ";" "\\;" CMAKE_PREFIX_PATH_STR "${CMAKE_PREFIX_PATH}")

find_package(absl QUIET)

if (NOT absl_FOUND)
        ExternalProject_Add(ABSL
                #DEPENDS LIBUV
                PREFIX            ${ABSL_ROOT}
                GIT_REPOSITORY    ${ABSL_GIT_URL}
                GIT_TAG           ${ABSL_GIT_TAG}
                CONFIGURE_COMMAND ${ABSL_CONFIGURE}
                BUILD_COMMAND     ${ABSL_MAKE}
                INSTALL_COMMAND   ${ABSL_INSTALL}
                # LOG_CONFIGURE     1
                # LOG_INSTALL       1
                CMAKE_ARGS          "-DBUILD_SHARED_LIBS=ON;-DBUILD_TESTING=OFF;-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE};-DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH_STR}"
        )
endif()

ADD_LIBRARY(absl SHARED IMPORTED GLOBAL)
SET_PROPERTY(TARGET absl PROPERTY IMPORTED_LOCATION ${ABSL_LIBRARIES})
add_dependencies(absl ABSL)

set(LIB_BIBRARY
        ${ABSL_LIBRARIES}
        ${LIB_BIBRARY})
set(LIB_DEPENDS
        "absl"
        ${LIB_DEPENDS})

#set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${ABSL_ROOT}/lib/pkgconfig/")


