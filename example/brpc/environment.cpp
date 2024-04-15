
#include <random>   // ::std::random_device
#include <stdio.h>  // ::vfprintf
#include <stdarg.h> // va_list

#include "environment.h"

// 弱依赖comlog，在未使用comlog时，补充屏显定义
__attribute__((weak)) int com_writelog(int, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ::vfprintf(stderr, fmt, args);
    va_end(args);
    ::fprintf(stderr, "\n");
    return 0;
}

BABYLON_NAMESPACE_BEGIN

static const char* random_device_name_default() {
#if __GLIBCXX__
    return "default";
#else // !__GLIBCXX__
    return "/dev/urandom";
#endif // !__GLIBCXX__
}

#if GLIBCXX_VERSION >= 1020200507
static const char* random_device_name_rdrand() {
    return "rdrand";
}
#endif // GLIBCXX_VERSION >= 1020200507

static const char* random_device_name_auto() {
// gcc10 默认使用的rdseed很大概率会在多线程环境有问题
// 如https://gcc.gnu.org/bugzilla/show_bug.cgi?id=94087
// 针对gcc10采用rdrand替换来规避问题
#if GLIBCXX_VERSION >= 1020200507
    try {
        ::std::random_device rd(random_device_name_rdrand());
        random_device_name = random_device_name_rdrand;
        return random_device_name();
    } catch (std::exception&) {
        // 由于指令集问题，老cpu可能不支持rdrand
        // 不过此时也没有rdseed的问题了，改回默认也没问题
    }
#endif // GLIBCXX_VERSION >= 1020200507
    random_device_name = random_device_name_default;
    return random_device_name();
}

const char*(*random_device_name)() = random_device_name_auto;

BABYLON_NAMESPACE_END
