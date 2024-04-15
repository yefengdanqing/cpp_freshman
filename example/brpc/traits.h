#ifndef BAIDU_FEED_MLARCH_BABYLON_REUSABLE_TRAITS_H
#define BAIDU_FEED_MLARCH_BABYLON_REUSABLE_TRAITS_H

#include <type_traits>

#include <stddef.h>

#include <helper.h>

// #include <base/logging.h>

// google::protobuf::Message命名很固定，用来支持对应的类型判定
// 独立前向声明用于支持不引入protobuf库的兼容场景
namespace google {
namespace protobuf {
class Message;
}
}

namespace baidu {
namespace feed {
namespace mlarch {
namespace babylon {

namespace internal {
namespace reusable_traits {

// 探测T.reconstruct(Args...)用来重建实例
template <typename T, typename... Args>
struct IsReconstructible {
    template <typename U = T>
    static auto checker(int32_t) -> decltype(
        ::std::declval<U>().reconstruct(::std::declval<Args>()...));
    template <typename U = T>
    static ::std::false_type checker(...);

    static constexpr bool value = !::std::is_same<decltype(checker<T>(0)), ::std::false_type>::value;
};
template <typename T, typename... Args, typename ::std::enable_if<IsReconstructible<T, Args...>::value, int32_t>::type = 0>
inline void reconstruct(T& instance, Args&&... args) {
    instance.reconstruct(::std::forward<Args>(args)...);
}

// 探测T.assign(Args...)用来重建实例
// 和构造函数同参数列表的assign函数是STL的常见风格
template <typename T, typename... Args>
struct IsAssignable {
    template <typename U = T>
    static auto checker(int32_t) -> decltype(
        ::std::declval<U>().assign(::std::declval<Args>()...));
    template <typename U = T>
    static ::std::false_type checker(...);

    static constexpr bool value = !::std::is_same<decltype(checker<T>(0)), ::std::false_type>::value;
};
template <typename T, typename... Args, typename ::std::enable_if<
    !IsReconstructible<T, Args...>::value
    && IsAssignable<T, Args...>::value, int32_t>::type = 0>
inline void reconstruct(T& instance, Args&&... args) {
    instance.assign(::std::forward<Args>(args)...);
}

// 探测T = U用来重建实例
template <typename T, typename U, typename ::std::enable_if<::std::is_assignable<T&, U>::value, int32_t>::type = 0>
inline void reconstruct(T& instance, U&& value) {
    instance = ::std::forward<U>(value);
}

// 探测T.clear()用来进行空参数重建
template <typename T>
struct IsClearable {
    template <typename U = T>
    static auto checker(int32_t) -> decltype(
        ::std::declval<U>().clear());
    template <typename U = T>
    static ::std::false_type checker(...);

    static constexpr bool value = !::std::is_same<decltype(checker<T>(0)), ::std::false_type>::value;
};
template <typename T, typename ::std::enable_if<IsClearable<T>::value, int32_t>::type = 0>
inline void reconstruct(T& instance) {
    instance.clear();
}

// trivial类型空参数重建无需操作
template <typename T, typename ::std::enable_if<
    !IsClearable<T>::value
    && ::std::is_trivial<T>::value, int32_t>::type = 0>
inline void reconstruct(T&) {
}

template <typename T, typename ::std::enable_if<!IsClearable<T>::value && !::std::is_trivial<T>::value
    && ::std::is_base_of<::google::protobuf::Message, T>::value, int32_t>::type = 0>
inline void reconstruct(T& instance) {
    instance.Clear();
}

template <typename T>
struct IsDefineNonVoidAllocationMetadata {
    template <typename U = T>
    static typename U::AllocationMetadata checker(int32_t);
    template <typename U = T>
    static void checker(...);

    static constexpr bool value = !::std::is_same<decltype(checker<T>(0)), void>::value;
    typedef decltype(checker<T>(0)) type;
};

struct EmptyAllocationMetadata {
    char zero_size_field[0];
};

}
}

template <typename T, typename A, typename RT>
struct BaseReusableTraits;

template <typename T, typename A, typename E = void>
struct ReusableTraits : public BaseReusableTraits<T, A, ReusableTraits<T, A>> {};

template <typename T, typename A, typename RT>
struct BaseReusableTraits {
    // 检测是否定义了T::AllocationMetadata且不为void
    // 自定义类型可以通过定义这个类型来表示自己可重用
    typedef internal::reusable_traits::IsDefineNonVoidAllocationMetadata<T> IsDefineNonVoidAllocationMetadata;

    // 由于void类型比较特殊，无法参与许多计算，比如void&等类型无效
    // 替换void为char[0]便于后续统一编码
    // 使用char[0]是由于它是少有的sizeof为0的类型
    // 且sizeof为0可以充分表示类型无信息量，可以用于确定use_metadata
    typedef typename ::std::conditional<IsDefineNonVoidAllocationMetadata::value,
            typename IsDefineNonVoidAllocationMetadata::type,
            internal::reusable_traits::EmptyAllocationMetadata>::type AllocationMetadata;

    // 是否可重用，默认trivial类型可重用
    // 此外定义了T::AllocationMetadata的类型也认为可重用
    static constexpr bool reusable = ::std::is_trivial<T>::value || IsDefineNonVoidAllocationMetadata::value;

    // 重建过程是否依赖AllocationMetadata
    // 不依赖时可以不调用update_allocation_metadata
    // 重建时也可以不传入有效的AllocationMetadata
    // 默认如果AllocationMetadata无信息量，则重建无需依赖
    static constexpr bool use_metadata() noexcept {
        return sizeof(typename ReusableTraits<T, A>::AllocationMetadata) > 0;
    }

    // 实际使用的allocator类型
    typedef typename A::template rebind<T>::other Allocator;

    // 更新AllocationMetadata，返回更新后的总大小
    template <typename U = T, typename ::std::enable_if<ReusableTraits<U, A>::use_metadata(), int32_t>::type = 0>
    inline static size_t update_allocation_metadata(const U& instance, typename ReusableTraits<U, A>::AllocationMetadata& metadata) noexcept {
        return instance.update_allocation_metadata(metadata);
    }
    template <typename U = T, typename ::std::enable_if<!ReusableTraits<U, A>::use_metadata(), int32_t>::type = 0>
    inline static size_t update_allocation_metadata(const U&, typename ReusableTraits<U, A>::AllocationMetadata&) noexcept {
        return 0;
    }

    // 使用AllocationMetadata和Allocator构造实例
    // 构造出的实例重现了AllocationMetadata所描述的容量和可重用内部实例
    template <typename U = T, typename ::std::enable_if<ReusableTraits<U, A>::use_metadata(), int32_t>::type = 0>
    inline static void construct_with_allocation_metadata(U* ptr, Allocator allocator,
            const typename ReusableTraits<U, A>::AllocationMetadata& metadata) noexcept {
        allocator.construct(ptr, metadata);
    }
    template <typename U = T, typename ::std::enable_if<!ReusableTraits<U, A>::use_metadata(), int32_t>::type = 0>
    inline static void construct_with_allocation_metadata(U* ptr, Allocator allocator,
            const typename ReusableTraits<U, A>::AllocationMetadata&) noexcept {
        allocator.construct(ptr);
    }

    // 集成allocate + construct + register_destructor
    template <typename U = T>
    inline static U* create_with_allocation_metadata(Allocator allocator, const typename ReusableTraits<U, A>::AllocationMetadata& metadata) {
        auto* instance = allocator.allocate(1);
        ReusableTraits<U, A>::construct_with_allocation_metadata(instance, allocator, metadata);
        allocator.register_destructor(instance);
        return instance;
    }

    // 判断是否支持在已经构造好的实例上，【再次构造】实例，用于支持实例重用
    template <typename... Args>
    struct Reconstructible {
        template <typename U = T>
        static auto checker(int32_t) -> decltype(
            internal::reusable_traits::reconstruct(::std::declval<U&>(), ::std::declval<Args>()...));
        template <typename U = T>
        static ::std::false_type checker(...);
        static constexpr bool value  = !::std::is_same<decltype(checker<T>(0)), ::std::false_type>::value;
    };
    // 可以【再次构造】的实例，执行【再次构造】
    template <typename... Args, typename ::std::enable_if<Reconstructible<Args...>::value, int32_t>::type = 0>
    inline static void reconstruct_instance(T& instance, Allocator, Args&&... args) {
        internal::reusable_traits::reconstruct(instance, ::std::forward<Args>(args)...);
    }
    // 不可【再次构造】的实例，先【销毁】再【构造】，支持容器对可重用和不可重用的元素统一代码处理
    template <typename... Args, typename ::std::enable_if<!Reconstructible<Args...>::value, int32_t>::type = 0>
    inline static void reconstruct_instance(T& instance, Allocator allocator, Args&&... args) {
        allocator.destroy(&instance);
        allocator.construct(&instance, ::std::forward<Args>(args)...);
    }
};

} // babylon
} // mlarch
} // feed
} // baidu

#endif // BAIDU_FEED_MLARCH_BABYLON_REUSABLE_TRAITS_H
