#pragma once

#include <allocator.h>

#ifdef GOOGLE_PROTOBUF_HAS_ARENAS
namespace google {
namespace protobuf {
namespace internal {
// protobuf为了避免用户错误地在非Arena内存上调用Arena版本的构造函数
// 将其设置了protected访问控制，并且从3.12开始将protoc生成的Message子类设置为final
// 但是为了实现MonotonicAllocator<T, Arena>，需要在已经分配好的内存上调用Arena版本的构造函数
// 这里利用了Arena对GenericTypeHandler<T>的friend声明来绕过访问了控制
// 通过Arena::CreateInArenaStorageInternal来达到目的，这也是protobuf::Map所使用的方法
// 采用了NeverUsed这个内部定义类型来特化GenericTypeHandler，确保这个特化不会产生副作用
template <>
class GenericTypeHandler<::baidu::feed::mlarch::babylon::NeverUsed> {
public:
    template <typename T, typename... Args>
    inline static void construct(T* ptr, ::google::protobuf::Arena* arena, Args&&... args) {
#if GOOGLE_PROTOBUF_VERSION >= 3006000
        ::google::protobuf::Arena::CreateInArenaStorageInternal(ptr, arena, std::true_type(), ::std::forward<Args>(args)...);
#else // GOOGLE_PROTOBUF_VERSION < 3006000
        ::google::protobuf::Arena::CreateInArenaStorageInternal(ptr, arena, true_type(), ::std::forward<Args>(args)...);
#endif // GOOGLE_PROTOBUF_VERSION < 3006000
    }
};
}
}
}
#endif // GOOGLE_PROTOBUF_HAS_ARENAS

namespace baidu {
namespace feed {
namespace mlarch {
namespace babylon {

#ifdef GOOGLE_PROTOBUF_HAS_ARENAS
///////////////////////////////////////////////////////////////////////////////
// SwissAllocator begin
template <typename T>
template <typename U, typename>
inline void SwissAllocator<T>::construct(U* ptr) {
    ::google::protobuf::Arena& arena = *this->underlying;
    ::google::protobuf::internal::GenericTypeHandler<NeverUsed>::construct(ptr, &arena);
}

template <typename T>
template <typename U, typename V, typename>
inline void SwissAllocator<T>::construct(U* ptr, V&& other) {
    ::google::protobuf::Arena& arena = *this->underlying;
    ::google::protobuf::internal::GenericTypeHandler<NeverUsed>::construct(ptr, &arena);
    *ptr = ::std::forward<V>(other);
}

template <typename T>
template <typename U>
inline void SwissAllocator<T>::destroy(U* ptr) noexcept {
    if (::google::protobuf::Arena::is_arena_constructable<U>::value
            && ::google::protobuf::Arena::is_destructor_skippable<U>::value) {
        return;
    }
    ptr->~U();
}

template <typename T>
template <typename U>
inline void SwissAllocator<T>::register_destructor(U* ptr) noexcept {
    if (::google::protobuf::Arena::is_arena_constructable<U>::value
            && ::google::protobuf::Arena::is_destructor_skippable<U>::value) {
        return;
    }
    this->underlying->register_destructor(ptr);
}
// SwissAllocator end
///////////////////////////////////////////////////////////////////////////////
#endif // GOOGLE_PROTOBUF_HAS_ARENAS

#ifdef GOOGLE_PROTOBUF_HAS_ARENAS
///////////////////////////////////////////////////////////////////////////////
// AreanAllocator begin
template <typename T>
inline typename MonotonicAllocator<T, ::google::protobuf::Arena>::pointer
MonotonicAllocator<T, ::google::protobuf::Arena>::allocate(size_type num) {
    return reinterpret_cast<pointer>(
        Arena::CreateArray<char>(this->underlying, sizeof(value_type) * num));
}

template <typename T>
template <typename U, typename Arg, typename ::std::enable_if<
    ::google::protobuf::Arena::is_arena_constructable<U>::value, int32_t>::type>
inline void MonotonicAllocator<T, ::google::protobuf::Arena>::construct(U* ptr, Arg&& arg) {
    construct<U>(ptr);
    *ptr = ::std::forward<Arg>(arg);
}


template <typename T>
template <typename U, typename ::std::enable_if<
    ::google::protobuf::Arena::is_arena_constructable<U>::value, int32_t>::type>
inline void MonotonicAllocator<T, ::google::protobuf::Arena>::construct(U* ptr) {
    ::google::protobuf::internal::GenericTypeHandler<NeverUsed>::construct(ptr, this->underlying);
}

template <typename T>
template <typename U, typename... Args, typename ::std::enable_if<
    !::google::protobuf::Arena::is_arena_constructable<U>::value
    && internal::reusable_allocator::Constructible<
        MonotonicAllocator<T, MonotonicAllocator<T, ::google::protobuf::Arena>>, U*,
        Args...>::value, int32_t>::type>
inline void MonotonicAllocator<T, ::google::protobuf::Arena>::construct(U* ptr, Args&&... args) {
    BaseType::construct(ptr, ::std::forward<Args>(args)...);
}

template <typename T>
template <typename U>
inline void MonotonicAllocator<T, ::google::protobuf::Arena>::register_destructor(U* ptr) {
    if (!::google::protobuf::Arena::is_destructor_skippable<U>::value) {
        this->underlying->OwnDestructor(ptr);
    }
}
// ArenaAllocator end
///////////////////////////////////////////////////////////////////////////////
#endif // GOOGLE_PROTOBUF_HAS_ARENAS

///////////////////////////////////////////////////////////////////////////////
// SerialAllocator begin
template <typename T>
inline typename MonotonicAllocator<T, StaticMemoryPool>::pointer
MonotonicAllocator<T, StaticMemoryPool>::allocate(size_type num) {
    return reinterpret_cast<pointer>(
        this->underlying->template create_aligned_buffer<alignof(T)>(sizeof(value_type) * num));
}

template <typename T>
template <typename U, typename... Args, typename ::std::enable_if<
    StaticMemoryPool::Constructible<U, Args...>::VALUE, int32_t>::type>
inline void MonotonicAllocator<T, StaticMemoryPool>::construct(U* ptr, Args&&... args) {
    new (reinterpret_cast<void*>(ptr)) U(*this->underlying, ::std::forward<Args>(args)...);
}

template <typename T>
template <typename U, typename... Args, typename ::std::enable_if<
    !StaticMemoryPool::Constructible<U, Args...>::VALUE
    && internal::reusable_allocator::Constructible<
        MonotonicAllocator<T, MonotonicAllocator<T, StaticMemoryPool>>, U*,
        Args...>::value, int32_t>::type>
inline void MonotonicAllocator<T, StaticMemoryPool>::construct(U* ptr, Args&&... args) {
    BaseType::construct(ptr, ::std::forward<Args>(args)...);
}

template <typename T>
template <typename U>
inline void MonotonicAllocator<T, StaticMemoryPool>::register_destructor(U* ptr) {
    this->underlying->register_destructor(ptr);
}
// SerialAllocator end
///////////////////////////////////////////////////////////////////////////////
} // babylon
} // mlarch
} // feed
} // baidu
