// #ifndef BAIDU_FEED_MLARCH_BABYLON_MEMORY_POOL_H
// #define BAIDU_FEED_MLARCH_BABYLON_MEMORY_POOL_H


// // #include <gflags/gflags.h>

// #include <list>
// #include <memory>
// #include <string_view>

// namespace baidu {
// namespace feed {
// namespace mlarch {
// namespace babylon {
//     struct NeverUsed {
//         NeverUsed() = delete;
//         NeverUsed(const NeverUsed&) = delete;
//         NeverUsed(NeverUsed&&) = delete;
//     };

// namespace internal {
// namespace memory_pool {
    
//    // 【内部使用】
//     // 一个带有meta的内存块，空间为[capacity, next, data...]
//     struct MemoryBlock {
//         // 获取不小于expected_size的一个合适的block_size
//         // 用于后续进行create调用，主要隐藏内部的meta信息大小
//         // 最终create分配时内存保持整齐，4KB前2进制扩展，即16, 32, 64, 128...
//         // 4KB后按照4KB扩展，即4KB，8KB，12KB，16KB...
//         inline static size_t get_block_size(size_t expected_size) noexcept;

//         // 构造一个MemoryBlock，由于空间布局特殊，合法的MemoryBlock只能通过此函数获得
//         inline static MemoryBlock* create(size_t capacity);

//         // 销毁一个MemoryBlock，避免被编译器错误判断sized-delete
//         // 只能通过此函数销毁
//         inline static void destroy(MemoryBlock* block);

//         // data区域的可用大小
//         size_t capacity;
//         // 用于链接多个MemoryBlock
//         MemoryBlock* next;
//         // 从这里开始，后面是实际可用的内存区
//         char data[0];
//     };
// }
// }

// // 单线程内存池，特化于支持
// // 短生命周期频繁申请的场景
// // clear时不会释放内存
// // 所有接口都【不是】线程安全的
// class StaticMemoryPool {
// public:
//     // 探测是否支持级联池构造
//     template <typename U, typename... Args>
//     struct Constructible {
//         static constexpr bool VALUE = ::std::is_constructible<U, StaticMemoryPool&, Args...>::value
//             && !::std::is_constructible<U, NeverUsed&, Args...>::value;
//     };

//     // 构造内存池
//     inline StaticMemoryPool() noexcept;
//     inline explicit StaticMemoryPool(size_t block_size) noexcept;
//     // 可以移动
//     inline StaticMemoryPool(StaticMemoryPool&& other) noexcept;
//     inline StaticMemoryPool& operator=(StaticMemoryPool&& other);
//     // 不可复制
//     inline StaticMemoryPool(const StaticMemoryPool&) noexcept = delete;
//     inline StaticMemoryPool& operator=(const StaticMemoryPool&) noexcept = delete;
//     // 析构时释放内存前，先析构创建过的对象
//     inline ~StaticMemoryPool() noexcept;

//     // ========================================================================
//     // 分配内存，得到的内存在clear或析构后失效
//     // 分配一块定长内存，对内容不进行初始化
//     inline char* create_buffer(size_t buffer_size);
//     // 分配对齐的内存，得到的内存在clear或析构后失效
//     template <uint16_t align_val>
//     inline char* create_aligned_buffer(size_t buffer_size);
//     inline char* create_aligned_buffer(size_t buffer_size, uint16_t align_val);
//     // 分配一块定长内存，用buffer中的内容拷贝进行初始化
//     inline char* create_buffer(::std::string_view buffer);
//     inline char* create_buffer(const char* buffer, size_t buffer_size);
//     // 分配一块定长内存，用c_str中的内容拷贝进行初始化
//     // 确保返回指针指向cstring即，确保'\0'结尾
//     inline char* create_cstring_buffer(::std::string_view view);
//     inline char* create_cstring_buffer(const char* c_str, size_t length);
//     // 分配一块定长内存，用c_str中的内容拷贝进行初始化
//     // 包装为std::string_view返回，简化一些情况下的代码
//     inline ::std::string_view create_cstring_view(::std::string_view view);
//     inline ::std::string_view create_cstring_view(const char* c_str, size_t length);
//     // inline ::base::StringPiece create_string_piece(::base::StringPiece piece);
//     // inline ::base::StringPiece create_string_piece(const char* c_str, size_t length);
//     // ========================================================================
    
//     // 创建对象，实际内存分配在pool内部，生命周期也在由pool托管
//     // T::T(StaticMemoryPool&, Args&&...)函数如果存在，会优先用来进行构造，否则使用T::T(Args&&...)
//     // 构造时探测如果支持StaticMemoryPool协议，就传入pool构造
//     template <typename T, typename... Args, typename ::std::enable_if<
//         ::std::is_constructible<T, StaticMemoryPool&, Args&&...>::value
//         // 探测如果支持NeverUsed作为参数构造，则具有万能构造函数
//         // 此时无法得知是否支持StaticMemoryPool协议，调用普通构造
//         // lijiang01 todo: 后续加入协议函数探测，解决需要万能构造的场景
//         && !::std::is_constructible<T, NeverUsed&, Args&&...>::value, int>::type = 0>
//     inline T* create_object(Args&&... args);

//     // 不支持的话正常构造
//     template <typename T, typename... Args, typename std::enable_if<
//         std::is_constructible<T, Args&&...>::value
//         && (!std::is_constructible<T, StaticMemoryPool&, Args&&...>::value
//         // 不支持StaticMemoryPool协议，或者无法判定为支持
//         // 比如具有万能构造函数的情况下，调用普通构造
//         // lijiang01 todo: 后续加入协议函数探测，解决需要万能构造的场景
//         || std::is_constructible<T, NeverUsed&, Args&&...>::value), int>::type = 0>
//     inline T* create_object(Args&&... args);

//     // 注册析构函数调用，在clear或者析构时统一调用
//     // 默认调用T::~T()，也可以制定特殊的析构函数
//     template <typename T>
//     inline void register_destructor(T* ptr);
//     inline void register_destructor(void* ptr, void(*destructor)(void*));

//     // 逻辑清空池中所有已分配的内容，但不会物理上释放内存
//     inline void clear() noexcept;

//     // 释放所有未真正使用的memory block
//     inline void shrink_to_fit() noexcept;

//     // 释放未使用的memory block，保留至少min_capacity
//     // 返回缩减之后的capacity
//     inline size_t shrink_to_fit(size_t min_capacity) noexcept;

//     // 读取block_size
//     inline size_t block_size() const noexcept;

//     //初始化后，提供重新设定block size接口
//     inline void set_block_size(size_t block_size) noexcept;

//     // 提供测试assert支持，判定pool生效
//     inline bool contain(const void* address) const noexcept;

//     // 提供测试assert支持，获取当前已使用空间大小
//     inline size_t size() const noexcept;

//     // 提供测试assert支持，获取当前已分配空间大小
//     inline size_t capacity() const noexcept;


// private:
//     typedef internal::memory_pool::MemoryBlock MemoryBlock;
//     struct Cleaner {
//         inline Cleaner(void(*destructor)(void*), void* pointer) noexcept;
//         inline Cleaner(Cleaner&& other) noexcept;
//         inline Cleaner(const Cleaner& other) noexcept = delete;
//         inline ~Cleaner() noexcept;

//         void (*_destructor)(void*);
//         void* _pointer;
//     };

//     template <typename T>
//     inline static void destruct_object(void* object) noexcept;

//     inline void compact_block(MemoryBlock*& block) noexcept;
//     inline void destroy_blocks_start_from(MemoryBlock* block);
//     template <uint16_t align_val_under_8>
//     inline void align_pos();
//     template <uint16_t align_val_under_8>
//     inline char* create_buffer_internal(size_t buffer_size);

//     size_t _block_size;
//     size_t _pos {0};
//     MemoryBlock* _head {nullptr};
//     MemoryBlock* _current_block {nullptr};

//     ::std::vector<Cleaner> _cleaners;
// };

// } // babylon
// } // mlarch
// } // feed
// } // baidu

// #endif //BAIDU_FEED_MLARCH_BABYLON_MEMORY_POOL_H


