#ifndef BAIDU_FEED_MLARCH_BABYLON_HELPER_H
#define BAIDU_FEED_MLARCH_BABYLON_HELPER_H

// #include <baidu/feed/mlarch/babylon/environment.h>
// #include <baidu/feed/mlarch/babylon/string_view.h>
// #include <baidu/feed/mlarch/babylon/lite/type_traits.h>

#include <atomic>
#include <memory>
#include <string_view>
#include "environment.h"

#include "absl/utility/utility.h"

namespace baidu {
namespace feed {
namespace mlarch {
namespace babylon {

// 搭配::std::unique_ptr使用，选择性生命周期控制
// 并提供定制的释放（比如通过回池来替换默认delete）
// T: 实例类型
// D: 释放器类型
// void (*F)(D*, T*): 实际调用此函数来释放
template <typename T, typename D = void, void (*F)(D*, T*) = nullptr>
class ConditionalDeleter {
public:
    // 构造时明确传入解分配器，传入nullptr视为非托管对象
    inline ConditionalDeleter(D* deallocator) noexcept;
    inline void operator()(T* ptr) const noexcept;
    // 查询是否是托管对象
    inline bool own() const noexcept;

private:
    D* _deallocator;
};

// 使用默认解分配（delete）的ConditionalDeleter
// 只进行选择性的释放
template <typename T, void (*F)(void*, T*)>
class ConditionalDeleter<T, void, F> {
public:
    // 构造时明确传入是否需要托管
    inline ConditionalDeleter(bool need_delete) noexcept;
    inline void operator()(T* ptr) const noexcept;
    // 查询是否是托管对象
    inline bool own() const noexcept;

private:
    bool _need_delete;
};

// 由于历史原因std::uses_allocator协议包括前置和后置两种情况
// 在stl中存在较多变参透传结构，即构造函数支持Args...
// 此时判定是否支持前缀或者后缀协议
// 以及Args...是否已经满足某个协议
// 相对繁琐，专门提供封装的判定方法
class AllocatorConstructHelper {
private:
    template <typename P, typename A, typename... Args>
    struct PairConstructible;
    template <typename P, typename A, typename... Args>
    struct PairCopyOrMoveConstructible;

public:
    // 判断是否可以通过std::use_allocator协议，使用A作为Allocator来构造T
    // 实际会依据T的实现情况采用前置或后置协议来构造，即
    // T(std::allocator_arg_t, A, Args...)
    // 或者
    // T(Args..., A)
    // 额外，针对std::pair进行特化处理，不依赖原本的构造函数
    // 而是递归对内部的first和second分别判断
    //
    // 在能够应用协议，或者std::pair的任意元素能够应用协议时
    // Constructible::USES_ALLOCATOR == true
    // 否则
    // Constructible::USES_ALLOCATOR == false
    // 
    // 在Constructible::USES_ALLOCATOR == true或者T(Args...)可用时
    // Constructible::value == true
    // 否则
    // Constructible::value == false
    template <typename T, typename A, typename... Args>
    class Constructible;

    ////////////////////////////////////////////////////////////////////////////
    // 统一签名为construct(T*, A, Args...)
    // 默认采用T(Args...)完成构造
    // 在满足容器化构造协议std::use_allocator时，即Constructible::USES_ALLOCATOR == true
    // 会改写构造参数，采用T(Args..., A)或者T(allocator_arg, A, Args...)做到分配器级联传递
    // 对于std::pair<F, S>，会分别尝试对其first和second进行协议判断和增补操作

    // 可前缀增补构造T(allocator_arg, A, Args...)
    template <typename T, typename A, typename... Args, typename ::std::enable_if<
        Constructible<T, A, Args&&...>::PREFIXED, int>::type = 0>
    inline static void construct(T* ptr, const A& allocator, Args&&... args) noexcept;

    // 可后缀增补构造T(Args..., A)
    template <typename T, typename A, typename... Args, typename ::std::enable_if<
        Constructible<T, A, Args&&...>::SUFFIXED, int>::type = 0>
    inline static void construct(T* ptr, const A& allocator, Args&&... args) noexcept;

    // 不可增补，直接构造T(Args...)
    template <typename T, typename A, typename... Args, typename ::std::enable_if<
        !Constructible<T, A, Args...>::USES_ALLOCATOR
        && !PairConstructible<T, A, Args...>::value, int>::type = 0>
    inline static void construct(T* ptr, A allocator, Args&&... args) noexcept;

    // 特化支持std::pair()
    template <typename P, typename A, typename = typename ::std::enable_if<
        PairConstructible<P, A>::value>::type>
    inline static void construct(P* pair, A allocator) noexcept;

    // 特化支持std::pair(const std::pair&)以及std::pair(std::pair&&)
    template <typename P, typename A, typename PP, typename ::std::enable_if<
        PairCopyOrMoveConstructible<P, A, PP>::value, int>::type = 0>
    inline static void construct(P* pair, A allocator, PP&& other) noexcept;

    // 特化支持原本不存在的std::pair(F)，F仅用来构造pair.first
    // 这个特化主要能够方便类似::std::map中的emplace的实现
    template <typename P, typename A, typename FF, typename ::std::enable_if<
        !PairCopyOrMoveConstructible<P, A, FF>::value && PairConstructible<P, A, FF>::value, int>::type = 0>
    inline static void construct(P* pair, A allocator, FF&& first) noexcept;

    // 特化支持std::pair(F, S)
    template <typename P, typename A, typename FF, typename SS, typename = typename ::std::enable_if<
        PairConstructible<P, A, FF, SS>::value>::type>
    inline static void construct(P* pair, A allocator, FF&& first, SS&& second) noexcept;

    // 特化支持std::pair(piecewise_construct_t, tuple, tuple)
    template <typename P, typename A, typename... FArgs, typename... SArgs, typename = typename ::std::enable_if<
        PairConstructible<P, A, FArgs..., SArgs...>::value>::type>
    inline static void construct(P* pair, A allocator, ::std::piecewise_construct_t,
            ::std::tuple<FArgs...> ftuple, ::std::tuple<SArgs...> stuple) noexcept;
    ////////////////////////////////////////////////////////////////////////////

private:
    template <typename T, typename... Args>
    struct AllocatorApplied;
    template <typename T, typename... Args>
    struct AllocatorSuffixed;
    template <typename T, typename... Args>
    struct AllocatorPrefixed;
    template <typename T>
    struct PairTraits;

    // BABYLON_DECLARE_MEMBER_INVOCABLE(allocate, AllocatorChecker);

    // 功能类似std::apply(construct, tuple<ptr, A, Args...>)
    // 但是能够在低于-std=c++17的场景下使用
    template <typename T, typename A, typename... Args, size_t... I, typename = typename ::std::enable_if<
        Constructible<T, A, Args...>::value>::type>
    inline static void construct_from_tuple_with_indexes(T* ptr, A allocator,
            ::std::tuple<Args...>& args_tuple, ::absl::index_sequence<I...>) noexcept;
};

// 补齐大小的结构体包装
template <typename T, size_t A>
class alignas(A) Aligned {
public:
    template <typename... Args>
    inline Aligned(Args&&... args) noexcept :
        _object(::std::forward<Args>(args)...) {}

    inline operator T&() noexcept {
        return _object;
    }

    inline operator const T&() const noexcept {
        return _object;
    }

    inline T& get() noexcept {
        return _object;
    }

private:
    T _object;
};

// 包装一个指针U*，呈现成指针T*
template <typename T, typename U = T>
class Pointer {
public:
    // 构造函数
    inline Pointer() noexcept = default;
    inline Pointer(U* object) noexcept;
    inline Pointer(const Pointer& other) noexcept = default;
    inline Pointer& operator=(const Pointer& other) noexcept = default;
    // 仿指针
    inline T& operator*() const noexcept;
    inline T* operator->() const noexcept;
    // 转换成T*使用
    inline operator T*() const noexcept;
    // 转换成U*使用
    inline operator U*() const noexcept;
    // delete指向内容
    inline void destroy();
    // 比较指针
    inline bool operator==(const Pointer& other) const noexcept;
    inline bool operator!=(const Pointer& other) const noexcept;

private:
    U* _object {nullptr};
};

// 涉及内部实现耦合，
// GCC4.8.2在未开启RTTI时未实现必要函数，考虑厂内大范围迁移了GCC8
// 就不做特化支持了
#if GLIBCXX_VERSION >= 820180726
// std::shared_ptr进行release_shared_ptr操作后可能有两种销毁方式
// 这是因为std::shared_ptr针对std::make_shared有特化处理导致
// 这里通过特化std::unique_ptr的deleter来表达这个不同
template <typename T>
class ReleasedSharedPtrDeleter {
public:
    inline ReleasedSharedPtrDeleter(
        ::std::_Sp_counted_base<::std::__default_lock_policy>* shared_count) noexcept;
    inline void operator()(T* ptr) const noexcept;
    // 当unique_ptr.deleter().releasable() == true时
    // 表明可以使用release得到原始指针，并自行delete销毁
    // 否则由于需要处理特化的内部结构，必须由ReleasedSharedPtrDeleter执行销毁
    inline bool releasable() const noexcept;

private:
    ::std::_Sp_counted_base<::std::__default_lock_policy>* _shared_count;
};

// 补全std::shared_ptr -> std::unique_ptr的转化，用于回收shared_ptr中的指针
// 相当于尝试进行shared_ptr::reset(nullptr)
// 但对引用计数到0的情况，并不销毁实例，改为包装unique_ptr返回
// 由于std::make_shared构造的shared_ptr经过了深度优化，此时无法采用裸指针返回
// 因此实际会返回定制析构器的unique_ptr，通过析构器可以判断是否可以进一步获得裸指针
template <typename T>
inline ::std::unique_ptr<T, ReleasedSharedPtrDeleter<T>> release_shared_ptr(::std::shared_ptr<T>& shared_ptr) noexcept;
#endif // GLIBCXX_VERSION >= 820180726

} // babylon
} // mlarch
} // feed
} // baidu

#endif // BAIDU_FEED_MLARCH_BABYLON_HELPER_H

