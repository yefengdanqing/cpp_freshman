#ifndef BAIDU_FEED_MLARCH_BABYLON_HELPER_HPP
#define BAIDU_FEED_MLARCH_BABYLON_HELPER_HPP

#include <helper.h>

namespace baidu {
namespace feed {
namespace mlarch {
namespace babylon {

///////////////////////////////////////////////////////////////////////////////
// ConditionalDeleter begin
//template <typename T, typename F>
template <typename T, typename D, void (*F)(D*, T*)>
inline ConditionalDeleter<T, D, F>::ConditionalDeleter(D* deallocator) noexcept :
    _deallocator(deallocator) {}

//template <typename T, typename F>
template <typename T, typename D, void (*F)(D*, T*)>
inline void ConditionalDeleter<T, D, F>::operator()(T* ptr) const noexcept {
    if (_deallocator != nullptr) {
        F(_deallocator, ptr);
    }
}

//template <typename T, typename F>
template <typename T, typename D, void (*F)(D*, T*)>
inline bool ConditionalDeleter<T, D, F>::own() const noexcept {
    return _deallocator != nullptr;
}

//template <typename T, typename F>
template <typename T, void (*F)(void*, T*)>
inline ConditionalDeleter<T, void, F>::ConditionalDeleter(bool need_delete) noexcept :
    _need_delete(need_delete) {}

//template <typename T, typename F>
template <typename T, void (*F)(void*, T*)>
inline void ConditionalDeleter<T, void, F>::operator()(T* ptr) const noexcept {
    if (_need_delete) {
        delete ptr;
    }
}

//template <typename T, typename F>
template <typename T, void (*F)(void*, T*)>
inline bool ConditionalDeleter<T, void, F>::own() const noexcept {
    return _need_delete;
}
// ConditionalDeleter end
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// AllocatorConstructHelper begin
// 用于提取pair对象的first和second类型，对非pair实例定义为特殊的NeverUsed
template <typename P>
struct AllocatorConstructHelper::PairTraits {
    using FirstType = NeverUsed;
    using SecondType = NeverUsed;
};

template <typename F, typename S>
struct AllocatorConstructHelper::PairTraits<::std::pair<F, S>> {
    using FirstType = F;
    using SecondType = S;
};

template <typename F, typename S>
struct AllocatorConstructHelper::PairTraits<::std::pair<F, S>&&> {
    using FirstType = F&&;
    using SecondType = S&&;
};

template <typename F, typename S>
struct AllocatorConstructHelper::PairTraits<::std::pair<F, S>&> {
    using FirstType = F&;
    using SecondType = S&;
};

template <typename F, typename S>
struct AllocatorConstructHelper::PairTraits<const ::std::pair<F, S>&> {
    using FirstType = const F&;
    using SecondType = const S&;
};

// 用于识别Args唯一，且是pair时是否可以用于目标pair的构造
template <typename P, typename A, typename... Args>
struct AllocatorConstructHelper::PairCopyOrMoveConstructible {
    static constexpr bool value = false;
    static constexpr bool USES_ALLOCATOR = false;
};

template <typename P, typename A, typename PP>
struct AllocatorConstructHelper::PairCopyOrMoveConstructible<P, A, PP> {
    using F = typename PairTraits<typename ::std::remove_cv<P>::type>::FirstType;
    using S = typename PairTraits<typename ::std::remove_cv<P>::type>::SecondType;
    using FF = typename PairTraits<PP>::FirstType;
    using SS = typename PairTraits<PP>::SecondType;
    static constexpr bool value = Constructible<F, A, FF>::value && Constructible<S, A, SS>::value;
    static constexpr bool USES_ALLOCATOR = value &&
        (Constructible<F, A, FF>::USES_ALLOCATOR || Constructible<S, A, SS>::USES_ALLOCATOR);
};

// 综合识别Args是否可以用于pair的构造
template <typename P, typename A, typename... Args>
struct AllocatorConstructHelper::PairConstructible {
    static constexpr bool value = false;
    static constexpr bool USES_ALLOCATOR = false;
};

template <typename P, typename A>
struct AllocatorConstructHelper::PairConstructible<P, A> {
    using F = typename PairTraits<typename ::std::remove_cv<P>::type>::FirstType;
    using S = typename PairTraits<typename ::std::remove_cv<P>::type>::SecondType;
    static constexpr bool value = Constructible<F, A>::value || Constructible<S, A>::value;
    static constexpr bool USES_ALLOCATOR = value
        && (Constructible<F, A>::USES_ALLOCATOR || Constructible<S, A>::USES_ALLOCATOR);
};

template <typename P, typename A, typename FF>
struct AllocatorConstructHelper::PairConstructible<P, A, FF> {
    using F = typename PairTraits<typename ::std::remove_cv<P>::type>::FirstType;
    using S = typename PairTraits<typename ::std::remove_cv<P>::type>::SecondType;
    static constexpr bool value = Constructible<F, A, FF>::value || Constructible<S, A>::value;
    static constexpr bool USES_ALLOCATOR = value
        && (Constructible<F, A, FF>::USES_ALLOCATOR || Constructible<S, A>::USES_ALLOCATOR);
};

template <typename P, typename A, typename FF, typename SS>
struct AllocatorConstructHelper::PairConstructible<P, A, FF, SS> {
    using F = typename PairTraits<typename ::std::remove_cv<P>::type>::FirstType;
    using S = typename PairTraits<typename ::std::remove_cv<P>::type>::SecondType;
    static constexpr bool value = Constructible<F, A, FF>::value || Constructible<S, A, SS>::value;
    static constexpr bool USES_ALLOCATOR = value
        && (Constructible<F, A, FF>::USES_ALLOCATOR || Constructible<S, A, SS>::USES_ALLOCATOR);
};

template <typename P, typename A, typename... FArgs, typename... SArgs>
struct AllocatorConstructHelper::PairConstructible<P, A, ::std::tuple<FArgs...>, ::std::tuple<SArgs...>> {
    using F = typename PairTraits<typename ::std::remove_cv<P>::type>::FirstType;
    using S = typename PairTraits<typename ::std::remove_cv<P>::type>::SecondType;
    static constexpr bool value = Constructible<F, A, FArgs...>::value || Constructible<S, A, SArgs...>::value;
    static constexpr bool USES_ALLOCATOR = value
        && (Constructible<F, A, FArgs...>::USES_ALLOCATOR || Constructible<S, A, SArgs...>::USES_ALLOCATOR);
};

template <typename T, typename A, typename... Args>
class AllocatorConstructHelper::Constructible {
private:
    // 元素可能自身带cv修饰，但是构造时没有差别
    using U = typename ::std::remove_cv<T>::type;
    // std::uses_allocator设计为支持可转换的Allocator即可，而无需类型严格匹配
    // 但是实践中有时未采用定义allocator_type的方法，而是模板特化来实现
    // 可是模板特化往往是严格匹配，而非可转换，这里rebind兼容一下
    using AA = typename ::std::allocator_traits<A>::template rebind_alloc<U>;
    static constexpr bool PREFIXED = ::std::uses_allocator<U, AA>::value
        // Args本身并未满足协议
        && !AllocatorApplied<U, Args...>::value
        // 前置增补后可满足协议
        && ::std::is_constructible<U, ::std::allocator_arg_t, AA, Args...>::value;
    static constexpr bool SUFFIXED = ::std::uses_allocator<U, AA>::value
        // Args本身并未满足协议
        && !AllocatorApplied<U, Args...>::value
        // 前置增补无效
        && !::std::is_constructible<U, ::std::allocator_arg_t, AA, Args...>::value
        // 后置增补后可满足协议
        && ::std::is_constructible<U, Args..., AA>::value;
public:
    // 可以前置增补或后置增补
    static constexpr bool USES_ALLOCATOR = PREFIXED || SUFFIXED
        // 或者满足pair特化增补
        || PairConstructible<U, AA, Args...>::USES_ALLOCATOR
        || PairCopyOrMoveConstructible<U, AA, Args...>::USES_ALLOCATOR;
    
    static constexpr bool value = USES_ALLOCATOR
        || PairConstructible<U, AA, Args...>::value
        || PairCopyOrMoveConstructible<U, AA, Args...>::value
        || ::std::is_constructible<U, Args...>::value;

    friend class AllocatorConstructHelper;
};

// 判断是否已经满足了前置或者后置协议，已经满足则无需增补
template <typename T, typename... Args>
struct AllocatorConstructHelper::AllocatorApplied {
    static constexpr bool value = AllocatorPrefixed<T, Args...>::value
        || AllocatorSuffixed<T, Args...>::value;
};

// 判断是否已经满足了前置协议
template <typename T, typename... Args>
struct AllocatorConstructHelper::AllocatorPrefixed {
    static constexpr bool value = false;
};

template <typename T, typename TAG, typename A, typename... Args>
struct AllocatorConstructHelper::AllocatorPrefixed<T, TAG, A, Args...> {
    //static constexpr bool value = ::std::uses_allocator<T, typename ::std::decay<A>::type>::value
    static constexpr bool value = ::std::uses_allocator<T, A>::value
        && ::std::is_convertible<TAG, ::std::allocator_arg_t>::value
        //&& AllocatorChecker<typename ::std::decay<A>::type, size_t>::value;
        && AllocatorChecker<A, size_t>::value;
};

// 判断是否已经满足了后置协议
template <typename T, typename... Args>
struct AllocatorConstructHelper::AllocatorSuffixed {
    static constexpr bool value = false;
};

// 唯一参数情况下检测是否是满足协议的分配器
template <typename T, typename A>
struct AllocatorConstructHelper::AllocatorSuffixed<T, A> {
    //static constexpr bool value = ::std::uses_allocator<T, typename ::std::decay<A>::type>::value
        //&& AllocatorChecker<typename ::std::decay<A>::type, size_t>::value;
    static constexpr bool value = ::std::uses_allocator<T, A>::value
        && AllocatorChecker<A, size_t>::value;
};

// 递归去掉前面的参数，直到终止在最后一个参数
template <typename T, typename A, typename... Args>
struct AllocatorConstructHelper::AllocatorSuffixed<T, A, Args...> {
    static constexpr bool value = AllocatorSuffixed<T, Args...>::value;
};

template <typename A, typename... Args>
struct AllocatorConstructHelper::Constructible<NeverUsed, A, Args...> {
    static constexpr bool PREFIXED = false;
    static constexpr bool SUFFIXED = false;
    static constexpr bool USES_ALLOCATOR = false;
    static constexpr bool value = false;
};

template <typename U, typename A, typename... Args, typename ::std::enable_if<
    AllocatorConstructHelper::Constructible<U, A, Args&&...>::PREFIXED, int>::type>
inline void AllocatorConstructHelper::construct(U* ptr, const A& allocator, Args&&... args) noexcept {
    new (const_cast<void*>(reinterpret_cast<const void*>(ptr))) U(::std::allocator_arg, allocator, ::std::forward<Args>(args)...);
}

template <typename U, typename A, typename... Args, typename ::std::enable_if<
    AllocatorConstructHelper::Constructible<U, A, Args&&...>::SUFFIXED, int>::type>
inline void AllocatorConstructHelper::construct(U* ptr, const A& allocator, Args&&... args) noexcept {
    new (const_cast<void*>(reinterpret_cast<const void*>(ptr))) U(::std::forward<Args>(args)..., allocator);
}

template <typename U, typename A, typename... Args, typename ::std::enable_if<
    !AllocatorConstructHelper::Constructible<U, A, Args...>::USES_ALLOCATOR
    && !AllocatorConstructHelper::PairConstructible<U, A, Args...>::value, int>::type>
inline void AllocatorConstructHelper::construct(U* ptr, A, Args&&... args) noexcept {
    new (const_cast<void*>(reinterpret_cast<const void*>(ptr))) U(::std::forward<Args>(args)...);
}

template <typename P, typename A, typename>
inline void AllocatorConstructHelper::construct(P* pair, A allocator) noexcept {
    construct(&pair->first, allocator);
    construct(&pair->second, allocator);
}

template <typename P, typename A, typename PP, typename ::std::enable_if<
    AllocatorConstructHelper::PairCopyOrMoveConstructible<P, A, PP>::value, int>::type>
inline void AllocatorConstructHelper::construct(P* pair, A allocator, PP&& other) noexcept {
    construct(&pair->first, allocator, ::std::forward<typename PairTraits<PP>::FirstType>(other.first));
    construct(&pair->second, allocator, ::std::forward<typename PairTraits<PP>::SecondType>(other.second));
}

template <typename P, typename A, typename FF, typename ::std::enable_if<
    !AllocatorConstructHelper::PairCopyOrMoveConstructible<P, A, FF>::value
    && AllocatorConstructHelper::PairConstructible<P, A, FF>::value, int>::type>
inline void AllocatorConstructHelper::construct(P* pair, A allocator, FF&& first) noexcept {
    construct(&pair->first, allocator, ::std::forward<FF>(first));
    construct(&pair->second, allocator);
}

template <typename P, typename A, typename FF, typename SS, typename>
inline void AllocatorConstructHelper::construct(P* pair, A allocator, FF&& first, SS&& second) noexcept {
    construct(&pair->first, allocator, ::std::forward<FF>(first));
    construct(&pair->second, allocator, ::std::forward<SS>(second));
}

template <typename P, typename A, typename... FArgs, typename... SArgs, typename>
inline void AllocatorConstructHelper::construct(P* pair, A allocator, ::std::piecewise_construct_t,
        ::std::tuple<FArgs...> ftuple, ::std::tuple<SArgs...> stuple) noexcept {
    construct_from_tuple_with_indexes(&pair->first, allocator, ftuple, ::absl::make_index_sequence<sizeof...(FArgs)>{});
    construct_from_tuple_with_indexes(&pair->second, allocator, stuple, ::absl::make_index_sequence<sizeof...(SArgs)>{});
}

template <typename T, typename A, typename... Args, size_t... I, typename>
inline void AllocatorConstructHelper::construct_from_tuple_with_indexes(T* ptr, A allocator,
        ::std::tuple<Args...>& args_tuple, ::absl::index_sequence<I...>) noexcept {
    construct(ptr, allocator, ::std::forward<Args>(::std::get<I>(args_tuple))...);
}
// AllocatorConstructHelper end
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Pointer begin
template <typename T, typename U>
inline Pointer<T, U>::Pointer(U* object) noexcept :
    _object(object) {}

template <typename T, typename U>
inline T& Pointer<T, U>::operator*() const noexcept {
    return *_object;
}

template <typename T, typename U>
inline T* Pointer<T, U>::operator->() const noexcept {
    return &static_cast<T&>(*_object);
}

template <typename T, typename U>
inline Pointer<T, U>::operator T*() const noexcept {
    return &static_cast<T&>(*_object);
}

template <typename T, typename U>
inline Pointer<T, U>::operator U*() const noexcept {
    return _object;
}

template <typename T, typename U>
inline void Pointer<T, U>::destroy() {
    delete _object;
}

template <typename T, typename U>
inline bool Pointer<T, U>::operator==(const Pointer& other) const noexcept {
    return _object == other._object;
}

template <typename T, typename U>
inline bool Pointer<T, U>::operator!=(const Pointer& other) const noexcept {
    return !(*this == other);
}
// Pointer end
///////////////////////////////////////////////////////////////////////////////

#if GLIBCXX_VERSION >= 820180726
template <typename T>
inline ReleasedSharedPtrDeleter<T>::ReleasedSharedPtrDeleter(
    ::std::_Sp_counted_base<::std::__default_lock_policy>* shared_count) noexcept :
        _shared_count(shared_count) {}

template <typename T>
inline void ReleasedSharedPtrDeleter<T>::operator()(T* ptr) const noexcept {
    if (_shared_count == nullptr) {
        delete ptr;
    } else {
        _shared_count->_M_add_ref_copy();
        _shared_count->_M_release();
    }
}

template <typename T>
inline bool ReleasedSharedPtrDeleter<T>::releasable() const noexcept {
    return _shared_count == nullptr;
}

namespace internal {
namespace helper {
// 由于实际实现release_shared_ptr时需要访问std::shared_ptr的私有成员
// 通过统一伪造成::std::shared_ptr<void>便于对应编译单元单独开启-fno-access-control
// 【hack】由于std::shared_ptr<T>对不同的类型T内存布局一致，这个转换是安全的
::std::tuple<void*, ::std::_Sp_counted_base<::std::__default_lock_policy>*>
release_shared_ptr(::std::shared_ptr<void>& shared_ptr,
    const ::std::type_info& make_shared_tag_type) noexcept;
}
}

template <typename T>
inline ::std::unique_ptr<T, ReleasedSharedPtrDeleter<T>>
release_shared_ptr(::std::shared_ptr<T>& shared_ptr) noexcept {
    auto result = internal::helper::release_shared_ptr(
        // 【hack】由于std::shared_ptr<T>对不同的类型T内存布局一致，这个转换是安全的
        reinterpret_cast<::std::shared_ptr<void>&>(shared_ptr),
        // 通过std::shared_ptr构造函数构造，和通过std::make_shared构造具有不同的内部实现
        // 需要通过内部接口传入typeid判断，实现中根据是否开启rtti，typeid获取方式不同
        // 这里做相应的适配
#if __cpp_rtti
        typeid(::std::_Sp_make_shared_tag)
#else // !__cpp_rtti
        ::std::_Sp_make_shared_tag::_S_ti()
#endif
    );
    return ::std::unique_ptr<T, ReleasedSharedPtrDeleter<T>>(
        reinterpret_cast<T*>(::std::get<0>(result)),
        ::std::get<1>(result));
}
#endif // GLIBCXX_VERSION >= 820180726

} // babylon
} // mlarch
} // feed
} // baidu

#endif // BAIDU_FEED_MLARCH_BABYLON_HELPER_HPP
