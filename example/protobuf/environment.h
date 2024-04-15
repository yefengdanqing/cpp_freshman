// #pragma once

// #include <absl/base/optimization.h> // ABSL_PREDICT_TRUE,ABSL_PREDICT_FALSE
// #include <time.h>                   // ::clock_gettime

// ////////////////////////////////////////////////////////////////////////////////
// // 预留一些可以由使用者定制的宏，由最终编译单元控制

// // 用于设置预期的缓存行大小，指导一些并发结构的对齐原则
// #ifndef BABYLON_CACHELINE_SIZE
// #define BABYLON_CACHELINE_SIZE 64
// #endif

// // 【已废弃】早期提供likely/unlikely内核风格宏，干扰较大，与boost库等还有冲突
// // 目前推荐统一到ABSL_PREDICT_TRUE/ABSL_PREDICT_FALSE上
// // 仅在-DBABYLON_DEFINE_EXPECT=1是定义likely/unlikely宏，兼容历史依赖
// #ifndef BABYLON_DEFINE_EXPECT
// #define BABYLON_DEFINE_EXPECT 0
// #endif

// ////////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////////
// // 一些用来做版本适配的宏

// // GCC版本，例如GCC 4.8.2 => 40802
// // 由于clang使用的FAKE的GCC_VERSION，基于GLIB的版本判断更通用
// #ifndef GCC_VERSION
// #define GCC_VERSION (__GNUC__ * 10000 \
//     + __GNUC_MINOR__ * 100 \
//     + __GNUC_PATCHLEVEL__)
// #endif // GCC_VERSION

// #ifndef CLANG_VERSION
// #ifdef __clang__
// #define CLANG_VERSION (__clang_major__ * 10000 \
//     + __clang_minor__ * 100 \
//     + __clang_patchlevel__)
// #endif // __clang__
// #endif // CLANG_VERSION

// // 用于确定是否使用libstdc++，以及对应的版本
// // 由于clang使用的FAKE的GCC_VERSION，基于GLIB的版本判断更通用
// // GLIBCXX_VERSION采用大版本号 + 发布日期来做版本确定，参考https://gcc.gnu.org/develop.html#timeline
// // >=420140522 == >=gcc4.8.3
// // >=820180726 == >=gcc8.2.0
// // >=1020200507 == >=gcc10.1.0
// #ifdef __GLIBCXX__
// #ifdef _GLIBCXX_RELEASE
// #define GLIBCXX_VERSION (_GLIBCXX_RELEASE * 100000000 + __GLIBCXX__)
// // GLIBCXX 7以前并未定义_GLIBCXX_RELEASE，需要通过发布时间判断一下
// // 不过由于公司标准环境只有GLIBCXX 4，先简单统一视为GLIBCXX 4
// #else // !_GLIBCXX_RELEASE
// #define GLIBCXX_VERSION (4 * 100000000 + __GLIBCXX__)
// #endif // !_GLIBCXX_RELEASE
// #endif // __GLIBCXX__

// // C++14对constexpr进行了重大升级，有些函数只有在C++14后才能够constexpr
// #ifndef CONSTEXPR_SINCE_CXX14
// #if __cplusplus >= 201402L
// #define CONSTEXPR_SINCE_CXX14 constexpr
// #else // __cplusplus < 201402L
// #define CONSTEXPR_SINCE_CXX14
// #endif // __cplusplus
// #endif // CONSTEXPR_SINCE_CXX14

// ////////////////////////////////////////////////////////////////////////////////

// // 简化名空间书写
// #define BABYLON_NAMESPACE_BEGIN namespace baidu { namespace feed { namespace mlarch { namespace babylon {
// #define BABYLON_NAMESPACE_END }}}}

// ////////////////////////////////////////////////////////////////////////////////
// // 【已废弃】早期提供likely/unlikely内核风格宏/函数，干扰较大

// #pragma push_macro("likely")
// #undef likely
// #pragma push_macro("unlikely")
// #undef unlikely

// BABYLON_NAMESPACE_BEGIN
// // 类似boost使用函数版本提供支持，也可以和boost兼容
// template <typename T>
// __attribute__((always_inline))
// inline constexpr bool likely(T&& expr) noexcept {
//     return ABSL_PREDICT_TRUE(expr);
// }

// template <typename T>
// __attribute__((always_inline))
// inline constexpr bool unlikely(T&& expr) noexcept {
//     return ABSL_PREDICT_FALSE(expr);
// }
// BABYLON_NAMESPACE_END

// #pragma pop_macro("likely")
// #pragma pop_macro("unlikely")

// // 显式开启likely宏定义
// #if BABYLON_DEFINE_EXPECT

// #ifndef likely
// #define likely(x) ABSL_PREDICT_TRUE(x)
// #endif // likely

// #ifndef unlikely
// #define unlikely(x) ABSL_PREDICT_FALSE(x)
// #endif // unlikely

// #endif // BABYLON_DEFINE_EXPECT

// ////////////////////////////////////////////////////////////////////////////////

// BABYLON_NAMESPACE_BEGIN
// extern const char*(*random_device_name)();
// BABYLON_NAMESPACE_END

// // 弱依赖comlog，如果有则输出到comlog，主要用于一些必要fatal信息的输出
// int com_writelog(int, const char* fmt, ...);

// #define BABYLON_LOG_FATAL_EVERY_SECOND(fmt, args...) { \
//     static int64_t last_log_time_s = 0; \
//     ::timespec spec; \
//     ::clock_gettime(CLOCK_MONOTONIC, &spec); \
//     int64_t now_s = spec.tv_sec; \
//     if (now_s > last_log_time_s) { \
//         ::com_writelog(1, fmt, ##args); \
//         last_log_time_s = now_s; \
//     } \
// }

// #if __cplusplus >= 201703L && GLIBCXX_VERSION >= 1020200507
// #define BABYLON_HAS_POLYMORPHIC_MEMORY_RESOURCE
// #endif
