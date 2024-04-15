#pragma once

#include <helper.h>
#include <memory_pool.h>
#include <memory_resource.h>

// #include <base/logging.h>

#include <google/protobuf/message.h>
#include <absl/utility/utility.h>

#include <type_traits>

namespace baidu {
namespace feed {
namespace mlarch {
namespace babylon {

namespace internal {
namespace reusable_allocator {
BABYLON_DECLARE_MEMBER_INVOCABLE(construct, Constructible);
}
}

template <typename T = void, typename A = MonotonicBufferResource>
struct MonotonicAllocator;

// 基础实现，用于定义单调配器的通用部分
// 一个单调分配器MonotonicAllocator<T, A>
// 可以通过继承MonotonicAllocator<T, MonotonicAllocator<T, A>>
// 得到大多数的分配器通用实现
template <typename T, typename A>
struct MonotonicAllocator<T, MonotonicAllocator<T, A>> {
    // void&是非法类型，替换成char[0]
    typedef typename ::std::conditional<::std::is_same<T, void>::value, char[0], T>::type ValueTypeForReference;

    // allocator必要的类型定义
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef ValueTypeForReference& reference;
    typedef const ValueTypeForReference& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    // allocator的rebind协议
    template <typename U>
    struct rebind {
        typedef MonotonicAllocator<U, A> other;
    };

    template <typename... Args>
    using Constructible = internal::reusable_allocator::Constructible<MonotonicAllocator<T, A>, T*, Args...>;

    // allocator只是底层资源的简单包装
    inline MonotonicAllocator(A& underlying) noexcept : underlying(&underlying) {}

    // allocator的rebind协议
    template <typename U>
    inline MonotonicAllocator(const MonotonicAllocator<U, A>& other) noexcept : underlying(other.underlying) {}

    // 分配连续num个T实例的内存，此处占位用，应由实际子类实现
    inline pointer allocate(size_type num) noexcept;

    // 单调分配器不需要释放内存
    inline void deallocate(T*, size_type) noexcept {}

    // 支持std::uses_allocator级联协议
    template <typename U, typename... Args, typename = typename ::std::enable_if<
        AllocatorConstructHelper::Constructible<U, MonotonicAllocator<U, A>, Args...>::value>::type>
    inline void construct(U* ptr, Args&&... args) {
        AllocatorConstructHelper::construct(ptr, MonotonicAllocator<U, A>(*underlying), ::std::forward<Args>(args)...);
    }

    // 析构接口
    template <typename U>
    inline void destroy(U* ptr) noexcept {
        ptr->~U();
    }

    // 分配不限定类型的nbytes内存，对齐到alignment
    //inline void* allocate_bytes(size_t nbytes, size_t alignment = alignof(::std::max_align_t));
    // 对应的空释放接口
    inline void deallocate_bytes(void*, size_t, size_t = alignof(::std::max_align_t)) noexcept {}

    template <typename U>
    inline U* allocate_object() noexcept {
        return MonotonicAllocator<U, A>(*underlying).allocate(1);
    }

    template <typename U>
    inline U* allocate_object(size_t num) noexcept {
        return MonotonicAllocator<U, A>(*underlying).allocate(num);
    }

    template <typename U>
    inline void deallocate_object(U*, size_t) noexcept {}

    template <typename U, typename... Args>
    inline U* new_object(Args&&... args) {
        auto* ptr = MonotonicAllocator<U, A>(*underlying).template allocate_object<U>();
        MonotonicAllocator<U, A>(*underlying).construct(ptr, ::std::forward<Args>(args)...);
        return ptr;
    }

    template <typename U>
    inline void delete_object(U* ptr) noexcept {
        MonotonicAllocator<U, A>(*underlying).template destroy(ptr);
    }

    template <typename U, typename... Args>
    inline U* create_object(Args&&... args) {
        auto* ptr = new_object<U>(::std::forward<Args>(args)...);
        MonotonicAllocator<T, A>(*underlying).template register_destructor(ptr);
        return ptr;
    }

    template <typename... Args>
    inline T* create(Args&&... args) {
        return MonotonicAllocator<T, A>(*underlying).template create_object<T>(::std::forward<Args>(args)...);
    }

    // 注册析构函数到内存池托管
    template <typename U>
    inline void register_destructor(U* ptr) noexcept {
        underlying->register_destructor(ptr);
    }

    // 底层内存池相同则相等
    inline bool operator==(const MonotonicAllocator& other) const noexcept {
        return underlying == other.underlying;
    }
    inline bool operator!=(const MonotonicAllocator& other) const noexcept {
        return underlying != other.underlying;
    }

    // 底层内存池
    A* underlying;
};

// 设计主要用来支持MonotonicBufferResource，M == MonotonicBufferResource时为多态实现
// M = sub class of MonotonicBufferResource时，提供绕过虚函数的实现
template <typename T, typename M>
struct MonotonicAllocator : public MonotonicAllocator<T, MonotonicAllocator<T, M>> {
    static_assert(::std::is_base_of<MonotonicBufferResource, M>::value,
            "default implement support MonotonicBufferResource only");
    typedef MonotonicAllocator<T, MonotonicAllocator<T, M>> Base;
    using Base::Base;

    // 提供类似std::pmr::polymorphic_allocator的构造接口，便于支持pmr容器
    inline MonotonicAllocator(M* resource) noexcept : Base(*resource) {}

    ////////////////////////////////////////////////////////////////////////////
    // 分配接口
    inline T* allocate(size_t num) noexcept {
        return reinterpret_cast<T*>(this->underlying->template allocate<alignof(T)>(sizeof(T) * num));
    }

    inline void* allocate_bytes(size_t nbytes, size_t alignment = alignof(::std::max_align_t)) noexcept {
        return this->underlying->allocate(nbytes, alignment);
    }

    template <typename U>
    inline U* allocate_object() noexcept {
        return reinterpret_cast<U*>(this->underlying->template allocate<alignof(U)>(sizeof(U)));
    }
    ////////////////////////////////////////////////////////////////////////////

#ifdef BABYLON_HAS_POLYMORPHIC_MEMORY_RESOURCE
    // 支持pmr容器
    template <typename U, typename... Args, typename = typename ::std::enable_if<
        AllocatorConstructHelper::Constructible<U, ::std::pmr::polymorphic_allocator<U>, Args...>::USES_ALLOCATOR
        >::type>
    inline void construct(U* ptr, Args&&... args) {
        AllocatorConstructHelper::construct(ptr, ::std::pmr::polymorphic_allocator<U>(this->underlying), ::std::forward<Args>(args)...);
    }
#endif

    // 代理到默认实现
    template <typename U, typename... Args, typename ::std::enable_if<
        internal::reusable_allocator::Constructible<Base, U*, Args...>::value
#ifdef BABYLON_HAS_POLYMORPHIC_MEMORY_RESOURCE
        && !AllocatorConstructHelper::Constructible<U, ::std::pmr::polymorphic_allocator<U>, Args...>::USES_ALLOCATOR
#endif
        , int>::type = 0>
    inline void construct(U* ptr, Args&&... args) {
        Base::construct(ptr, ::std::forward<Args>(args)...);
    }

    template <typename U>
    inline void register_destructor(U* ptr) noexcept {
        this->underlying->register_destructor(ptr);
    }

    inline M* resource() const noexcept {
        return this->underlying;
    }
};

// 特化支持SwissMemoryResource，实现stl和protobuf的统一支持
template <typename T>
struct MonotonicAllocator<T, SwissMemoryResource> :
        public MonotonicAllocator<T, MonotonicAllocator<T, SwissMemoryResource>> {
    typedef MonotonicAllocator<T, MonotonicAllocator<T, SwissMemoryResource>> Base;
    using Base::Base;

    // 提供类似std::pmr::polymorphic_allocator的构造接口，便于支持pmr容器
    inline MonotonicAllocator(SwissMemoryResource* resource) noexcept : Base(*resource) {}

    ////////////////////////////////////////////////////////////////////////////
    // 分配接口
    inline T* allocate(size_t num) noexcept {
        return reinterpret_cast<T*>(this->underlying->template allocate<alignof(T)>(sizeof(T) * num));
    }

    inline void* allocate_bytes(size_t nbytes, size_t alignment = alignof(::std::max_align_t)) noexcept {
        return this->underlying->allocate(nbytes, alignment);
    }

    template <typename U>
    inline U* allocate_object() noexcept {
        return reinterpret_cast<U*>(this->underlying->template allocate<alignof(U)>(sizeof(U)));
    }
    ////////////////////////////////////////////////////////////////////////////

#ifdef GOOGLE_PROTOBUF_HAS_ARENAS
    // 支持protobuf::Message
    template <typename U, typename = typename ::std::enable_if<
        ::google::protobuf::Arena::is_arena_constructable<U>::value>::type>
    inline void construct(U* ptr);

    template <typename U, typename V, typename = typename ::std::enable_if<
        ::google::protobuf::Arena::is_arena_constructable<U>::value>::type>
    inline void construct(U* ptr, V&& other);

    // protobuf::Message对于Arena构造版本不能调用析构函数，需要特判
    template <typename U>
    inline void destroy(U* ptr) noexcept;

    template <typename U>
    inline void register_destructor(U* ptr) noexcept;
#endif

#ifdef BABYLON_HAS_POLYMORPHIC_MEMORY_RESOURCE
    // 支持pmr容器
    template <typename U, typename... Args, typename = typename ::std::enable_if<
        AllocatorConstructHelper::Constructible<U, ::std::pmr::polymorphic_allocator<U>, Args...>::USES_ALLOCATOR
        >::type>
    inline void construct(U* ptr, Args&&... args) {
        AllocatorConstructHelper::construct(ptr, ::std::pmr::polymorphic_allocator<U>(this->underlying), ::std::forward<Args>(args)...);
    }
#endif

    // 代理到默认实现
    template <typename U, typename... Args, typename ::std::enable_if<
        internal::reusable_allocator::Constructible<Base, U*, Args...>::value
#ifdef BABYLON_HAS_POLYMORPHIC_MEMORY_RESOURCE
        && !AllocatorConstructHelper::Constructible<U, ::std::pmr::polymorphic_allocator<U>, Args...>::USES_ALLOCATOR
#endif
        , int>::type = 0>
    inline void construct(U* ptr, Args&&... args) {
        Base::construct(ptr, ::std::forward<Args>(args)...);
    }

    inline SwissMemoryResource& resource() const noexcept {
        return *this->underlying;
    }
};

template <typename T = void>
using SwissAllocator = MonotonicAllocator<T, SwissMemoryResource>;

template <typename A>
class PolymorphicReusableMemoryResource;

#ifdef GOOGLE_PROTOBUF_HAS_ARENAS
template <>
class PolymorphicReusableMemoryResource<::google::protobuf::Arena> : public ReusableMemoryResource {
public:
    inline PolymorphicReusableMemoryResource() = default;
    inline PolymorphicReusableMemoryResource(const ::google::protobuf::ArenaOptions& options) :
        arena(options) {}

    virtual void* do_allocate(::std::size_t bytes, ::std::size_t alignment) noexcept override {
        switch (alignment) {
        case 1:
        case 2:
        case 4:
        case 8: {
                return ::google::protobuf::Arena::CreateArray<char>(&arena, bytes);
            }
        default: {
                LOG_EVERY_SECOND(FATAL) << "Arena can not support alignment " << alignment << " not in [1, 2, 4, 8]";
                return ::google::protobuf::Arena::CreateArray<char>(&arena, bytes);
            }
        }
    }

    virtual void do_register_destructor(void* ptr, void (*destructor)(void*)) noexcept override {
        return arena.OwnCustomDestructor(ptr, destructor);
    }

    virtual void do_release() noexcept override {
        arena.Reset();
    }

    inline size_t capacity() const noexcept {
        return arena.SpaceAllocated();
    }

    inline size_t used() const noexcept {
        return arena.SpaceUsed();
    }

    inline void clear_and_shrink(size_t reserved) {
        // TODO(lijiang01): 精确使用arena的headsize
        size_t block_size_need = reserved + 96;
        if (block_size_need > buffer.size()) {
            arena.~Arena();
            buffer.resize(block_size_need);
            ::google::protobuf::ArenaOptions options;
            options.initial_block = &buffer[0];
            options.initial_block_size = buffer.size();
            new (&arena) ::google::protobuf::Arena(options);
        } else {
            arena.Reset();
        }
    }

    ::std::vector<char> buffer;
    ::google::protobuf::Arena arena;
};

// 包装内存池的分配器，典型特征是无需进行内存释放
// T为分配器目标类型，A为底层内存池的类型
// 包装protobuf的Arena到allocator实现
// 由于Arena的限制，不支持align > 8byte的情况
template <typename T>
struct MonotonicAllocator<T, ::google::protobuf::Arena> : public MonotonicAllocator<T, MonotonicAllocator<T, ::google::protobuf::Arena>> {
    typedef ::google::protobuf::Arena Arena;
    typedef MonotonicAllocator<T, MonotonicAllocator<T, Arena>> BaseType;
    typedef typename BaseType::value_type value_type;
    typedef typename BaseType::pointer pointer;
    typedef typename BaseType::size_type size_type;
    using BaseType::BaseType;

    inline MonotonicAllocator(PolymorphicReusableMemoryResource<::google::protobuf::Arena>& resource) noexcept :
        MonotonicAllocator(resource.arena) {}

    // 从Arena分配内存
    inline pointer allocate(size_type num);

    // 支持ArenaConstructable协议
    template <typename U, typename Arg, typename ::std::enable_if<
        Arena::is_arena_constructable<U>::value, int32_t>::type = 0>
    inline void construct(U* ptr, Arg&& arg);

    template <typename U, typename ::std::enable_if<Arena::is_arena_constructable<U>::value, int32_t>::type = 0>
    inline void construct(U* ptr);

    // 继承基类构造
    template <typename U, typename... Args, typename ::std::enable_if<
        !Arena::is_arena_constructable<U>::value
        && internal::reusable_allocator::Constructible<BaseType, U*, Args...>::value, int32_t>::type = 0>
    inline void construct(U* ptr, Args&&... args);

    // 注册析构函数到Arena托管
    template <typename U>
    inline void register_destructor(U* ptr);
};
// 简化包装Arena分配器的写法
template <typename T = void>
using ArenaAllocator = MonotonicAllocator<T, ::google::protobuf::Arena>;
#endif // GOOGLE_PROTOBUF_HAS_ARENAS

template <>
class PolymorphicReusableMemoryResource<StaticMemoryPool> : public ReusableMemoryResource {
public:
    virtual void* do_allocate(::std::size_t bytes, ::std::size_t alignment) noexcept override {
        switch (alignment) {
        case 1: {
                return pool.create_aligned_buffer<1>(bytes);
            }
        case 2: {
                return pool.create_aligned_buffer<2>(bytes);
            }
        case 4: {
                return pool.create_aligned_buffer<4>(bytes);
            }
        case 8: {
                return pool.create_aligned_buffer<8>(bytes);
            }
        default: {
                return pool.create_aligned_buffer(bytes, alignment);
            }
        }
    }

    virtual void do_register_destructor(void* ptr, void (*destructor)(void*)) noexcept override {
        return pool.register_destructor(ptr, destructor);
    }

    virtual void do_release() noexcept override {
        pool.clear();
    }

    inline size_t capacity() const noexcept {
        return pool.capacity();
    }

    inline size_t used() const noexcept {
        return pool.size();
    }

    inline void clear_and_shrink(size_t reserved) {
        pool.clear();
        pool.shrink_to_fit(reserved);
    }

    StaticMemoryPool pool;
};

template <typename T>
struct MonotonicAllocator<T, StaticMemoryPool> : public MonotonicAllocator<T, MonotonicAllocator<T, StaticMemoryPool>> {
    typedef MonotonicAllocator<T, MonotonicAllocator<T, StaticMemoryPool>> BaseType;
    typedef typename BaseType::value_type value_type;
    typedef typename BaseType::pointer pointer;
    typedef typename BaseType::size_type size_type;
    using BaseType::BaseType;

    inline MonotonicAllocator(PolymorphicReusableMemoryResource<StaticMemoryPool>& resource) noexcept :
        MonotonicAllocator(resource.pool) {}

    // 从StaticMemoryPool分配内存
    inline pointer allocate(size_type num);

    // 支持StaticMemoryPool协议
    template <typename U, typename... Args, typename ::std::enable_if<
        StaticMemoryPool::Constructible<U, Args...>::VALUE, int32_t>::type = 0>
    inline void construct(U* ptr, Args&&... args);

    // 继承基类构造
    template <typename U, typename... Args, typename ::std::enable_if<
        !StaticMemoryPool::Constructible<U, Args...>::VALUE
        && internal::reusable_allocator::Constructible<BaseType, U*, Args...>::value, int32_t>::type = 0>
    inline void construct(U* ptr, Args&&... args);

    // 注册析构函数到StaticMemoryPool托管
    template <typename U>
    inline void register_destructor(U* ptr);
};
// 简化包装StaticMemoryPool分配器的写法
template <typename T = void>
using SerialAllocator = MonotonicAllocator<T, StaticMemoryPool>;

// TODO(lijiang01): 少量模块对ReusableAllocator这个名字还有依赖，先define兼容一下
//                  后续改名后去掉
#define ReusableAllocator MonotonicAllocator

} // babylon
} // mlarch
} // feed
} // baidu

// #include <baidu/feed/mlarch/babylon/reusable/allocator.hpp>
