#pragma once

#include <environment.h>

#include <baidu/feed/mlarch/babylon/lite/reusable/memory_resource.h>
#include <baidu/feed/mlarch/babylon/memory_pool.h>

#include <google/protobuf/message.h>

namespace baidu {
namespace feed {
namespace mlarch {
namespace babylon {

// 多功能内存资源
// 支持多线程安全，而且能够以Arena方式呈现的内存资源
// 重在统一支持了stl/protobuf等上层容器，典型用来支持RPC中单次请求级的容器内存管理
class SwissMemoryResource : public SharedMonotonicBufferResource {
public:
    inline SwissMemoryResource() noexcept :
            SwissMemoryResource(SystemPageAllocator::instance()) {}

    inline SwissMemoryResource(PageAllocator& page_allocator) noexcept :
            SharedMonotonicBufferResource(page_allocator)
#ifdef GOOGLE_PROTOBUF_HAS_ARENAS
        , _arena(make_arena_options())
#endif
        {}

#ifdef GOOGLE_PROTOBUF_HAS_ARENAS
    inline operator ::google::protobuf::Arena&() noexcept {
        return _arena;
    }
#endif

private:
#ifdef GOOGLE_PROTOBUF_HAS_ARENAS
    // 用来注册给ArenaOption的分配/释放函数，本身的功能采用new/delete实现
    // 针对拦截hack未能生效的版本，能降级回原始的Arena实现
    static void* arena_block_alloc(size_t size) noexcept;
    static void arena_block_dealloc(void* ptr, size_t size) noexcept;

    inline ::google::protobuf::ArenaOptions make_arena_options() noexcept {
        ::google::protobuf::ArenaOptions options;
        // 在initial_block为nullptr时，注入initial_block_size是一个无效参数
        // 在拦截函数中通过识别block_alloc是否为SwissMemoryResource::arena_block_alloc
        // 可以准确判断是否隶属于SwissMemoryResource，并利用注入的this指针执行特殊操作
        options.initial_block = nullptr;
        options.initial_block_size = reinterpret_cast<size_t>(this);
        options.block_alloc = arena_block_alloc;
        options.block_dealloc = arena_block_dealloc;
        return options;
    }

    virtual void do_release() noexcept override;

    ::google::protobuf::Arena _arena;
#endif
};

// 相比『重用』而言，『单调』这个命名更符合特性，也和STL表述更统一
// 引入typedef兼容既有的使用者
typedef MonotonicBufferResource ReusableMemoryResource;

} // babylon
} // mlarch
} // feed
} // baidu
