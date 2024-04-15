// #ifndef BAIDU_FEED_MLARCH_BABYLON_MEMORY_POOL_HPP
// #define BAIDU_FEED_MLARCH_BABYLON_MEMORY_POOL_HPP

// #include <memory_pool.h>

// // DECLARE_uint64(babylon_buffer_block_size);

// namespace baidu {
// namespace feed {
// namespace mlarch {
// namespace babylon {

// namespace internal {
// namespace memory_pool {
// ///////////////////////////////////////////////////////////////////////////////
// // MemoryBlock begin
// inline size_t MemoryBlock::get_block_size(size_t expected_size) noexcept {
//     const size_t min_block_size = sizeof(MemoryBlock) + expected_size;
//     size_t block_size;
//     if (min_block_size > (4 << 10)) {
//         block_size = (min_block_size + (4 << 10) - 1) &
//             static_cast<size_t>(-(4 << 10));
//     } else {
//         size_t remain = min_block_size;
//         block_size = 1;
//         while (remain > 1) {
//             block_size <<= 1;
//             remain >>= 1;
//         }
//         if (block_size < min_block_size) {
//             block_size <<= 1;
//         }
//     }
//     return block_size - sizeof(MemoryBlock);
// }

// inline MemoryBlock* MemoryBlock::create(size_t capacity) {
//     auto* block = reinterpret_cast<MemoryBlock*>(
//         (::operator new)(sizeof(MemoryBlock) + capacity));
//     block->capacity = capacity;
//     block->next = nullptr;
//     return block;
// }

// inline void MemoryBlock::destroy(MemoryBlock* block) {
// #if __cpp_sized_deallocation
//     (::operator delete)(reinterpret_cast<void*>(block), sizeof(MemoryBlock) + block->capacity);
// #else
//     (::operator delete)(reinterpret_cast<void*>(block));
// #endif
// }
// // MemoryBlock end
// ///////////////////////////////////////////////////////////////////////////////
// }
// }

// ///////////////////////////////////////////////////////////////////////////////
// // StaticMemoryPool begin
// inline StaticMemoryPool::StaticMemoryPool() noexcept :
//     StaticMemoryPool(1024) {}

// inline StaticMemoryPool::StaticMemoryPool(size_t block_size) noexcept { 
//     set_block_size(block_size);
// }

// inline StaticMemoryPool::StaticMemoryPool(StaticMemoryPool&& other) noexcept :
//         _block_size(other._block_size),
//         _pos(other._pos),
//         _head(other._head),
//         _current_block(other._current_block),
//         _cleaners(::std::move(other._cleaners)) {
//     other._pos = 0;
//     other._head = nullptr;
//     other._current_block = nullptr;
// }

// inline StaticMemoryPool& StaticMemoryPool::operator=(StaticMemoryPool&& other) {
//     clear();
//     destroy_blocks_start_from(_head);

//     _block_size = other._block_size;
//     _pos = other._pos;
//     _head = other._head;
//     _current_block = other._current_block;
//     _cleaners = ::std::move(other._cleaners);

//     other._pos = 0;
//     other._head = nullptr;
//     other._current_block = nullptr;
//     return *this;
// }

// inline StaticMemoryPool::~StaticMemoryPool() noexcept {
//     _cleaners.clear();
//     destroy_blocks_start_from(_head);
// }

// inline char* StaticMemoryPool::create_buffer(size_t buffer_size) {
//     return create_buffer_internal<1>(buffer_size);
// }

// template <uint16_t align_val>
// inline char* StaticMemoryPool::create_aligned_buffer(size_t buffer_size) {
//     return create_aligned_buffer(buffer_size, align_val);
// }

// // 8字节以内create_buffer_internal可以直接处理
// template <>
// inline char* StaticMemoryPool::create_aligned_buffer<1>(size_t buffer_size) {
//     return create_buffer_internal<1>(buffer_size);
// }
// template <>
// inline char* StaticMemoryPool::create_aligned_buffer<2>(size_t buffer_size) {
//     return create_buffer_internal<2>(buffer_size);
// }
// template <>
// inline char* StaticMemoryPool::create_aligned_buffer<4>(size_t buffer_size) {
//     return create_buffer_internal<4>(buffer_size);
// }
// template <>
// inline char* StaticMemoryPool::create_aligned_buffer<8>(size_t buffer_size) {
//     return create_buffer_internal<8>(buffer_size);
// }

// inline char* StaticMemoryPool::create_buffer(::std::string_view view) {
//     return create_buffer(view.data(), view.size());
// }

// inline char* StaticMemoryPool::create_aligned_buffer(size_t buffer_size, uint16_t align_val) {
//     auto* buffer = create_aligned_buffer<1>(buffer_size + align_val);
//     auto* aligned_buffer = reinterpret_cast<char*>(
//         (reinterpret_cast<uint64_t>(buffer) + align_val - 1) & static_cast<uint64_t>(-align_val));
//     if (&_current_block->data[_pos] == &buffer[buffer_size + align_val]) {
//         _pos -= align_val - (aligned_buffer - buffer);
//     }
//     return aligned_buffer;
// }

// inline char* StaticMemoryPool::create_buffer(const char* buffer, size_t buffer_size) {
//     auto buffer_start = create_buffer(buffer_size);
//     memcpy(buffer_start, buffer, buffer_size);
//     return buffer_start;
// }

// inline char* StaticMemoryPool::create_cstring_buffer(::std::string_view view) {
//     return create_cstring_buffer(view.data(), view.size());
// }

// inline char* StaticMemoryPool::create_cstring_buffer(const char* c_str, size_t length) {
//     static char empty_cstring = '\0';
//     // 空串特殊处理
//     if (ABSL_PREDICT_FALSE(length == 0)) {
//         return &empty_cstring;
//     }
//     // 拷贝并补充'\0'结尾
//     auto buffer_start = create_buffer(length + 1);
//     memcpy(buffer_start, c_str, length);
//     buffer_start[length] = '\0';
//     return buffer_start;
// }

// inline ::base::StringPiece StaticMemoryPool::create_string_piece(::base::StringPiece piece) {
//     return create_string_piece(piece.data(), piece.size());
// }

// inline ::base::StringPiece StaticMemoryPool::create_string_piece(const char* c_str, size_t length) {
//     return ::base::StringPiece(create_cstring_buffer(c_str, length), length);
// }

// inline ::std::string_view StaticMemoryPool::create_cstring_view(::std::string_view view) {
//     return create_cstring_view(view.data(), view.size());
// }

// inline ::std::string_view StaticMemoryPool::create_cstring_view(const char* c_str, size_t length) {
//     // 空串特殊处理
//     if (ABSL_PREDICT_FALSE(length == 0)) {
//         return ::std::string_view();
//     }
//     // 拷贝并补充'\0'结尾
//     auto buffer_start = create_buffer(length + 1);
//     memcpy(buffer_start, c_str, length);
//     buffer_start[length] = '\0';
//     return ::std::string_view(buffer_start, length);
// }

// template <typename T, typename... Args, typename ::std::enable_if<
//     ::std::is_constructible<T, StaticMemoryPool&, Args&&...>::value
//     && !::std::is_constructible<T, NeverUsed&, Args&&...>::value, int>::type>
// inline T* StaticMemoryPool::create_object(Args&&... args) {
//     auto buffer = create_aligned_buffer<alignof(T)>(sizeof(T));
//     if (!::std::is_trivially_destructible<T>::value) {
//         _cleaners.emplace_back(&destruct_object<T>, buffer);
//     }
//     return new (reinterpret_cast<void*>(buffer)) T(*this, ::std::forward<Args>(args)...);
// }

// template <typename T, typename... Args, typename ::std::enable_if<
//     ::std::is_constructible<T, Args&&...>::value
//     && (!::std::is_constructible<T, StaticMemoryPool&, Args&&...>::value
//     || ::std::is_constructible<T, NeverUsed&, Args&&...>::value), int>::type>
// inline T* StaticMemoryPool::create_object(Args&&... args) {
//     auto buffer = create_aligned_buffer<alignof(T)>(sizeof(T));
//     if (!::std::is_trivially_destructible<T>::value) {
//         _cleaners.emplace_back(&destruct_object<T>, buffer);
//     }
//     return new (reinterpret_cast<void*>(buffer)) T(::std::forward<Args>(args)...);
// }

// inline StaticMemoryPool::Cleaner::Cleaner(void (*destructor)(void*), void* pointer) noexcept :
//     _destructor(destructor), _pointer(pointer) {}

// inline StaticMemoryPool::Cleaner::Cleaner(Cleaner&& other) noexcept :
//     _destructor(other._destructor), _pointer(other._pointer) {
//     other._destructor = nullptr;
//     other._pointer = nullptr;
// }

// inline StaticMemoryPool::Cleaner::~Cleaner() noexcept {
//     if (_destructor != nullptr) {
//         _destructor(_pointer);
//     }
// }

// inline void StaticMemoryPool::clear() noexcept {
//     _cleaners.clear();
//     if (_head != nullptr) {
//         compact_block(_head);
//         _current_block = _head;
//     }
//     _pos = 0;
// }

// template <typename T>
// inline void StaticMemoryPool::destruct_object(void* object) noexcept {
//     reinterpret_cast<T*>(object)->~T();
// }

// template <typename T>
// inline void StaticMemoryPool::register_destructor(T* ptr) {
//     if (!::std::is_trivially_destructible<T>::value) {
//         register_destructor(ptr, &destruct_object<T>);
//     }
// }

// inline void StaticMemoryPool::register_destructor(void* ptr, void(*destructor)(void*)) {
//     _cleaners.emplace_back(destructor, ptr);
// }

// inline void StaticMemoryPool::shrink_to_fit() noexcept {
//     if (_current_block != nullptr) {
//         if (_current_block == _head && _pos == 0) {
//             destroy_blocks_start_from(_head);
//             _head = _current_block = nullptr;
//         } else {
//             destroy_blocks_start_from(_current_block->next);
//             _current_block->next = nullptr;
//         }
//     }
// }

// inline size_t StaticMemoryPool::shrink_to_fit(size_t min_capacity) noexcept {
//     size_t capacity = 0;
//     if (_current_block != nullptr) {
//         auto* block = _head;
//         if (min_capacity == 0) {
//             clear();
//             destroy_blocks_start_from(_head);
//             _head = _current_block = nullptr;
//         } else {
//             capacity += _current_block->capacity;
//             for (; block != _current_block; block = block->next) {
//                 capacity += block->capacity;
//             }

//                 while (capacity < min_capacity && block->next != nullptr) {
//                 block = block->next;
//                 capacity += block->capacity;
//             }
//             destroy_blocks_start_from(block->next);
//             block->next = nullptr;
//         }
//     }
//     return capacity;
// }

// inline size_t StaticMemoryPool::block_size() const noexcept {
//     return _block_size;
// }
// inline void StaticMemoryPool::set_block_size(size_t block_size) noexcept {
//     _block_size = MemoryBlock::get_block_size(
//         block_size < sizeof(MemoryBlock) ?
//             sizeof(MemoryBlock) : block_size - sizeof(MemoryBlock)); 
// }
// inline bool StaticMemoryPool::contain(const void* address) const noexcept {
//     for (auto* block = _head; block != _current_block; block = block->next) {
//         if (block->data <= address && address < block->data + block->capacity) {
//             return true;
//         }
//     }

//     return _current_block != nullptr &&
//         _current_block->data <= address && address < _current_block->data + _pos;
// }

// inline size_t StaticMemoryPool::size() const noexcept {
//     size_t size = _pos;
//     for (auto* block = _head; block != _current_block; block = block->next) {
//         size += block->capacity;
//     }
//     return size;
// }

// inline size_t StaticMemoryPool::capacity() const noexcept {
//     size_t capacity = 0;
//     for (auto* block = _head; block != nullptr; block = block->next) {
//         capacity += block->capacity;
//     }
//     return capacity;
// }

// inline void StaticMemoryPool::compact_block(MemoryBlock*& block) noexcept {
//     if (_block_size <= block->capacity) {
//         return;
//     }

//     size_t capacity = block->capacity;
//     MemoryBlock* next_block = block->next;
//     while (next_block != nullptr && capacity < _block_size) {
//         capacity += next_block->capacity;
//         auto* to_be_destroyed_block = next_block;
//         next_block = next_block->next;
//         MemoryBlock::destroy(to_be_destroyed_block);
//     }

//     MemoryBlock::destroy(block);
//     block = MemoryBlock::create(_block_size);
//     block->next = next_block;
// }

// inline void StaticMemoryPool::destroy_blocks_start_from(MemoryBlock* block) {
//     while (block != nullptr) {
//         auto* to_be_destroyed_block = block;
//         block = block->next;
//         MemoryBlock::destroy(to_be_destroyed_block);
//     }
// }

// template <uint16_t align_val_under_8>
// inline void StaticMemoryPool::align_pos() {
//     _pos = (_pos + align_val_under_8 - 1) & static_cast<size_t>(-align_val_under_8);
// }

// // 1字节无需对齐，用于统一代码处理无需对齐的情况
// template <>
// inline void StaticMemoryPool::align_pos<1>() {
// }

// template <uint16_t align_val_under_8>
// inline char* StaticMemoryPool::create_buffer_internal(size_t buffer_size) {
//     if (ABSL_PREDICT_FALSE(_current_block == nullptr)) {
//         _current_block = _head = MemoryBlock::create(_block_size);
//     }
//     // 移动_pos对齐到下一个8以内的对齐点
//     align_pos<align_val_under_8>();
//     // block空间足够时直接分配返回
//     if (_current_block->capacity >= buffer_size + _pos) {
//         auto pos = _pos;
//         _pos += buffer_size;
//         //LOG(NOTICE) << "alloc 1 " << (void*)&_current_block->data[pos] << " to " << (void*)&_current_block->data[pos + buffer_size];
//         return &_current_block->data[pos];
//     // 空间不足，但是一个block可以容纳的另起一个新block
//     // 8以内的对齐点是默认满足的，不再做特殊处理
//     } else if (buffer_size <= _block_size) {
//         auto& next_block = _current_block->next;
//         // 重用block
//         if (next_block != nullptr) {
//             compact_block(next_block);
//             _current_block = next_block;
//         // block用尽，创建一个新的block
//         } else {
//             next_block = MemoryBlock::create(_block_size);
//             _current_block = next_block;
//         }
//         _pos = buffer_size;
//         //LOG(NOTICE) << "alloc 2 " << (void*)&_current_block->data[0] << " to " << (void*)&_current_block->data[buffer_size];
//         return &_current_block->data[0];
//     // 超过block size的在头部分配独立block
//     // 8以内的对齐点是默认满足的，不再做特殊处理
//     } else {
//         auto* block = MemoryBlock::create(buffer_size);
//         block->next = _head;
//         _head = block;
//         _block_size = MemoryBlock::get_block_size(buffer_size * 1.1f);
//         //LOG(NOTICE) << "alloc 3 " << (void*)&_head->data[0] << " to " << (void*)&_head->data[buffer_size];
//         return &_head->data[0];
//     }
// }
// // StaticMemoryPool end
// ///////////////////////////////////////////////////////////////////////////////

// } // babylon
// } // mlarch
// } // feed
// } // baidu

// #endif //BAIDU_FEED_MLARCH_BABYLON_MEMORY_POOL_HPP
