#ifndef BAIDU_FEED_MLARCH_BABYLON_DYNAMIC_STRUCT_H
#define BAIDU_FEED_MLARCH_BABYLON_DYNAMIC_STRUCT_H
// #include <baidu/feed/mlarch/babylon/lite/type_traits.h>
// #include <baidu/feed/mlarch/babylon/reusable/traits.h>
// #include <baidu/feed/mlarch/babylon/reusable/allocator.h>
// #include <baidu/feed/mlarch/babylon/reusable.h>
// #include <baidu/feed/mlarch/babylon/any.h>

#include "type_traits.h"
#include "memory_pool.h"



#include <unordered_map>
#include <memory>
#include <google/protobuf/message.h>
#include <mutex>


namespace baidu {
namespace feed {
namespace mlarch {
namespace babylon {
#ifdef GOOGLE_PROTOBUF_HAS_ARENAS
using ::google::protobuf::Arena;
#endif
struct FieldAccessor;
// BABYLON_DECLARE_STATIC_INVOCABLE(get_struct_schema, is_dynamic_structible)
namespace internal {
namespace dynamic_struct {
// BABYLON_DECLARE_MEMBER_INVOCABLE(clear, Clearable);
class BaseSchema;
typedef baidu::feed::mlarch::babylon::FieldAccessor FieldAccessor;
enum FieldType { DYNAMIC_STRUCT, DYNAMIC_STRUCT_POINTER, OTHER };
struct Field {
    
    Field() = default;
    Field(size_t offset,
          const std::string& name,
          const Id* type_id,
          FieldType field_type,
          const BaseSchema* schema):
            offset(offset),
            name(name),
            type_id(type_id),
            field_type(field_type),
            schema(schema){}
    size_t offset;
    std::string name;
    const Id* type_id;
    FieldType field_type;
    const BaseSchema* schema;
    // Any(*to_any_func)(const void*);
};
class BaseSchema {
public:
    // T 是指针类型，*T支持动态结构体协议
    template <
            typename T,
            typename A = void,
            typename std::enable_if<
                    std::is_pointer<T>::value &&
                            is_dynamic_structible<typename std::remove_cv<
                                    typename std::remove_pointer<T>::type>::type>::value,
                    int>::type = 0>
    inline const FieldAccessor register_field(const std::string&, size_t offset = std::string::npos);
    // T 是指针类型，*T不支持动态结构体协议
    template <
            typename T,
            typename A = void,
            typename std::enable_if<
                    std::is_pointer<T>::value &&
                            !is_dynamic_structible<typename std::remove_cv<
                                    typename std::remove_pointer<T>::type>::type>::value,
                    int>::type = 0>
    inline const FieldAccessor register_field(const std::string&, size_t offset = std::string::npos);
    // T 不是指针类型，T支持动态结构体协议
    template <
            typename T,
            typename A = void,
            typename std::enable_if<
                    !std::is_pointer<T>::value && is_dynamic_structible<T>::value,
                    int>::type = 0>
    inline const FieldAccessor register_field(const std::string&, size_t offset = std::string::npos);
    // T 不是指针类型，T不支持动态结构体协议
    template <
            typename T,
            typename A = void,
            typename std::enable_if<
                    !std::is_pointer<T>::value && !is_dynamic_structible<T>::value,
                    int>::type = 0>
    inline const FieldAccessor register_field(const std::string&, size_t offset = std::string::npos);
    inline const FieldAccessor get_field(const std::string&) const;
    inline const FieldAccessor get_field(const std::string&, int& err) const;
    inline const std::unordered_map<std::string, Field> *get_all_fields() const;
    inline void destroy_all(char*) const noexcept;
    inline void construct_all(char*, void*) const noexcept;
    inline void construct_with_metadata_all(char*, void*, const std::vector<void*>&) const noexcept;
    inline size_t update_metadata(const char*, const std::vector<void*>) const noexcept;
    inline void clear_all(char*, void*) const noexcept;
    inline void destroy_metadata(void* meta_ptr, size_t idx) const noexcept {
        if (_metadata_deletor[idx] != nullptr) {
            _metadata_deletor[idx](meta_ptr);
        }
    }
    inline size_t get_field_size() const noexcept {
        return _field_map.size();
    }
    inline const std::vector<void* (*)()>& get_create_meta() const noexcept {
        return _metadata_creator;
    }
    inline size_t allocate_align() const {
        return _allocate_align;
    }
    inline size_t allocate_size() const {
        return _allocate_size;
    }
    inline bool is_trivally_destructiable() const {
        return _is_trivially_destructible;
    }

    inline int set_size_and_align(size_t size, size_t align) {
        _allocate_align = ::std::max(_allocate_align, align);
        _unaligned_allocate_size += size;
        _allocate_size = (_unaligned_allocate_size + _allocate_align - 1) &
                static_cast<size_t>(-_allocate_align);
        return 0;
    }
protected:
    template <typename T, typename A>
    inline const FieldAccessor register_field(
            const std::string&,
            FieldType type,
            size_t offset,
            BaseSchema*) noexcept;
    inline BaseSchema(size_t init_align, size_t init_align_size) :
            _allocate_align(init_align),
            _allocate_size(init_align_size),
            _base_offset(init_align_size),
            _unaligned_allocate_size(init_align_size) {}

    template <typename T>
    inline static void destroy(void* ptr) noexcept {
        reinterpret_cast<T*>(ptr)->~T();
    }
    inline static void fill_with_zero(void* ptr, void*, const void*, size_t size) noexcept {
        __builtin_memset(ptr, 0, size);
    }
    template <typename T>
    inline static void construct(void* ptr, void*, const void*, size_t) noexcept {
        new (ptr) T();
    }
    template <typename T, typename A, typename std::enable_if<!std::is_trivially_destructible<T>::value, int>::type = 0>
    inline static void construct_with_pool(void* ptr, void* pool, const void*, size_t) noexcept {
        typename ReusableTraits<T, MonotonicAllocator<T, A>>::Allocator allocator(
                *(reinterpret_cast<A*>(pool)));
        allocator.construct(reinterpret_cast<T*>(ptr));
        allocator.register_destructor(reinterpret_cast<T*>(ptr));
    }
    template <typename T, typename A, typename std::enable_if<std::is_trivially_destructible<T>::value, int>::type = 0>
    inline static void construct_with_pool(void* ptr, void* pool, const void*, size_t) noexcept {
        typename ReusableTraits<T, MonotonicAllocator<T, A>>::Allocator allocator(
                *(reinterpret_cast<A*>(pool)));
        allocator.construct(reinterpret_cast<T*>(ptr));
    }
    template <typename T, typename A>
    inline static void* create_meta() noexcept {
        typedef typename std::conditional<std::is_void<A>::value, StaticMemoryPool, A>::type
                AllocateType;
        auto ptr = new typename ReusableTraits<T, MonotonicAllocator<T, AllocateType>>::
                AllocationMetadata();
        return ptr;
    }
    template <typename T, typename A>
    inline static void delete_meta(void* ptr) noexcept {
        typedef typename std::conditional<std::is_void<A>::value, StaticMemoryPool, A>::type
                AllocateType;
        delete reinterpret_cast<typename ReusableTraits<T, MonotonicAllocator<T, AllocateType>>::
                                        AllocationMetadata*>(ptr);
    }
    template <typename T, typename A>
    inline static size_t update_metadata(const void* instance, void* metadata_ptr) noexcept {
        typedef typename std::conditional<std::is_void<A>::value, StaticMemoryPool, A>::type
                AllocateType;
        auto t_instance = reinterpret_cast<const T*>(instance);
        return ReusableTraits<T, MonotonicAllocator<T, AllocateType>>::update_allocation_metadata(
                *t_instance,
                *reinterpret_cast<typename ReusableTraits<T, MonotonicAllocator<T, AllocateType>>::
                                          AllocationMetadata*>(metadata_ptr));
    }
    template <
            typename T,
            typename A,
            typename std::enable_if<
                    ReusableTraits<
                            T,
                            MonotonicAllocator<
                                    T,
                                    typename std::conditional<
                                            std::is_void<A>::value,
                                            StaticMemoryPool,
                                            A>::type>>::use_metadata(),
                    int>::type = 0>
    inline static void construct_with_metadata(
            void* instance,
            void* pool,
            const void* metadata,
            size_t) noexcept {
        typedef typename std::conditional<std::is_void<A>::value, StaticMemoryPool, A>::type
                AllocateType;
        typename ReusableTraits<T, MonotonicAllocator<T, AllocateType>>::Allocator allocator(
                *(reinterpret_cast<AllocateType*>(pool)));
        ReusableTraits<T, MonotonicAllocator<T, AllocateType>>::construct_with_allocation_metadata(
                reinterpret_cast<T*>(instance),
                allocator,
                *reinterpret_cast<const typename ReusableTraits<
                        T,
                        MonotonicAllocator<T, AllocateType>>::AllocationMetadata*>(metadata));
    }
    template <
            typename T,
            typename A,
            typename std::enable_if<
                    !std::is_void<A>::value &&
                            !ReusableTraits<
                                    T,
                                    MonotonicAllocator<
                                            T,
                                            typename std::conditional<
                                                    std::is_void<A>::value,
                                                    StaticMemoryPool,
                                                    A>::type>>::use_metadata(),
                    int>::type = 0>
    inline static void construct_with_metadata(
            void* instance,
            void* pool,
            const void*,
            size_t) noexcept {
        construct_with_pool<T, A>(instance, pool, nullptr, 0);
    }
    template <
            typename T,
            typename A,
            typename std::enable_if<std::is_void<A>::value, int>::type = 0>
    inline static void construct_with_metadata(
            void* instance,
            void*,
            const void*,
            size_t) noexcept {
        construct<T>(instance, nullptr, nullptr, 0);
    }
    template <
            typename T,
            typename A,
            typename std::enable_if<!std::is_void<A>::value, int>::type = 0>
    inline static void reconstruct(void* instance, void* pool, const void*, size_t) noexcept {
        typedef typename std::conditional<std::is_void<A>::value, StaticMemoryPool, A>::type
                AllocateType;
        auto t_instance = reinterpret_cast<T*>(instance);
        typename ReusableTraits<T, MonotonicAllocator<T, AllocateType>>::Allocator allocator(
                *(reinterpret_cast<AllocateType*>(pool)));
        ReusableTraits<T, MonotonicAllocator<T, AllocateType>>::reconstruct_instance(
                *t_instance, allocator);
    }
    template <
            typename T,
            typename A,
            typename ::std::enable_if<Clearable<T>::value && std::is_void<A>::value, int32_t>::
                    type = 0>
    inline static void reconstruct(void* ptr, void*, const void*, size_t) noexcept {
        reinterpret_cast<T*>(ptr)->clear();
    }
    template <
            typename T,
            typename A,
            typename ::std::enable_if<!Clearable<T>::value && std::is_void<A>::value, int32_t>::
                    type = 0>
    inline static void reconstruct(void* ptr, void*, const void*, size_t) noexcept {
        reinterpret_cast<T*>(ptr)->~T();
        new (ptr) T();
    }
    template<typename T, typename ::std::enable_if<!std::is_array<T>::value ,int>::type = 0>
    // inline static Any to_any(const void* ptr) {
    //     const T* var = reinterpret_cast<const T*>(ptr);
    //     Any any;
    //     any.ref<T>(*var);
    //     return any;
    // }
    // template<typename T, typename ::std::enable_if<std::is_array<T>::value ,int>::type = 0>
    // inline static Any to_any(const void*) {
    //     LOG(FATAL) << "this field is array, can not to be any!!";
    //     Any any;
    //     return any;
    // }
    template <
            typename T,
            typename A,
            typename std::enable_if<std::is_void<A>::value, int>::type = 0>
    inline void add_constructor(const Field& field);
    template <
            typename T,
            typename A,
            typename std::enable_if<!std::is_void<A>::value, int>::type = 0>
    inline void add_constructor(const Field& field);
    template <typename T, typename A, typename std::enable_if<std::is_trivially_destructible<T>::value || !std::is_void<A>::value, int>::type = 0>
    inline void add_destructor(const Field& field);
    template <typename T, typename A, typename std::enable_if<!std::is_trivially_destructible<T>::value && std::is_void<A>::value, int>::type = 0>
    inline void add_destructor(const Field& field);
    template <typename T, typename A, typename std::enable_if<std::is_trivial<T>::value, int>::type = 0>
    inline void add_cleaner(const Field& field);
    template <typename T, typename A, typename std::enable_if<!std::is_trivial<T>::value, int>::type = 0>
    inline void add_cleaner(const Field& field);
    template <typename T, typename A>
    inline void add_meta_data();
    template <typename T, typename A>
    inline void add_update_metadata(const Field& field);
    template <typename T, typename A>
    inline void add_contruct_with_metadata(const Field& field);
    template <typename T, typename A>
    inline void add_recontruct(const Field& field);
    template <typename T>
    inline size_t add_field_and_caculate_size() noexcept {
        size_t offset = _unaligned_allocate_size;
        offset = (offset + alignof(T) - 1) & static_cast<size_t>(-alignof(T));
        _unaligned_allocate_size = offset + sizeof(T);
        _allocate_align = ::std::max(_allocate_align, alignof(T));
        _allocate_size = (_unaligned_allocate_size + _allocate_align - 1) &
                static_cast<size_t>(-_allocate_align);
        return offset - _base_offset;
    }
    std::unordered_map<std::string, Field> _field_map;
    std::vector<::std::tuple<size_t, void (*)(void*)>> _destructors;
    ::std::vector<::std::tuple<size_t, size_t, void (*)(void*, void*, const void*, size_t)>>
            _constructors;
    ::std::vector<::std::tuple<size_t, size_t, void (*)(void*, void*, const void*, size_t)>>
            _cleaners;
    std::vector<void* (*)()> _metadata_creator;
    std::vector<void (*)(void*)> _metadata_deletor;
    std::vector<::std::tuple<size_t, size_t (*)(const void*, void*)>> _metadata_updator;
    std::vector<::std::tuple<size_t, size_t, void (*)(void*, void*, const void*, size_t)>>
            _with_meta_constructor;
    
    
    // std::vector<::std::tuple<size_t, void (*)(void*, void*)>> reconstructor;
    size_t _allocate_align;
    size_t _allocate_size;
    size_t _base_offset;
    size_t _unaligned_allocate_size;
    bool _is_trivially_destructible{true};
    std::mutex _mutex;
    friend FieldAccessor;
};
}  // namespace dynamic_struct
}  // namespace internal
using internal::dynamic_struct::FieldType;
struct FieldAccessor {
    inline FieldAccessor(const internal::dynamic_struct::Field& field) :
            offset(field.offset),
            field_type(field.field_type),
            field_ptr(&field) {}
    size_t offset;
    const FieldType field_type{FieldType::OTHER};
    const internal::dynamic_struct::Field* field_ptr{nullptr};
};
// define DynamicStruct
class DynamicStruct {
public:
    struct Schema : public internal::dynamic_struct::BaseSchema {
        struct Meta {
            const Schema* const schema{nullptr};
        };
        inline Schema() : internal::dynamic_struct::BaseSchema(alignof(Meta), sizeof(Meta)) {}
    };
    struct AllocationMetadata {
        std::vector<void*> metadatas;
        const DynamicStruct::Schema* schema{nullptr};
        inline ~AllocationMetadata() {
            for (size_t i = 0; i < metadatas.size(); ++i) {
                if (metadatas[i] != nullptr && schema != nullptr) {
                    schema->destroy_metadata(metadatas[i], i);
                }
            }
        }
    };
    DynamicStruct() = delete;
    DynamicStruct(DynamicStruct&&) = delete;
    DynamicStruct(const DynamicStruct&) = delete;
    DynamicStruct& operator=(DynamicStruct&&) = delete;
    DynamicStruct& operator=(const DynamicStruct&) = delete;
    inline ~DynamicStruct() noexcept {
        hidden_schema()->destroy_all(_data);
    }
    inline static void operator delete(void* ptr) {
        auto* meta = &reinterpret_cast<DynamicStruct*>(ptr)->hidden_meta();
#if __cpp_aligned_new && __cpp_sized_deallocation
        (::operator delete)(meta, meta->schema->allocate_size(), std::align_val_t(meta->schema->allocate_align()));
#elif __cpp_aligned_new
        (::operator delete)(meta, std::align_val_t(meta->schema->allocate_align()));
#else // !__cpp_aligned_new
        ::free(meta);
#endif // !__cpp_aligned_new
    }
    inline static void operator delete(void* ptr, size_t) {
        operator delete(ptr);
    }
    inline static std::unique_ptr<DynamicStruct> create(const Schema&);

    template <typename A, typename ::std::enable_if<::std::is_constructible<MonotonicAllocator<DynamicStruct, A>, A*>::value, int32_t>::type = 0>
    inline static DynamicStruct* create_with_pool(const Schema&, A*);

    template <typename T>
    inline static T* to_struct(DynamicStruct* dynamic_struct) {
        return reinterpret_cast<T*>(dynamic_struct);
    }
    template <typename T>
    inline static DynamicStruct* struct_to_dynamic(T* struct_ptr) {
        return reinterpret_cast<DynamicStruct*>(struct_ptr);
    }
    template <typename T>
    inline static const T* to_struct(const DynamicStruct* dynamic_struct) {
        return reinterpret_cast<const T*>(dynamic_struct);
    }
    template <typename T>
    inline static const DynamicStruct* struct_to_dynamic(const T* struct_ptr) {
        return reinterpret_cast<const DynamicStruct*>(struct_ptr);
    }
    template <typename T>
    inline T* get(const FieldAccessor& field) {
        return reinterpret_cast<T*>(&_data[field.offset]);
    }
    //嗅探 field
    //指向的对象是否实现了动态结构体协议，如果实现了，将对象转化成动态结构体返回。达到动态结构体链式调用的目的
    // e.g  ds->get_dynamic_struct(f1)->get_dynamic_struct(f2)->get_dynamic_struct(f3)
    inline DynamicStruct* get_dynamic_struct(const FieldAccessor& field);
    inline const DynamicStruct* cget_dynamic_struct(const FieldAccessor& field) const;
    //兼容AnyRecord接口
    template <typename T>
    inline const T* cget(const FieldAccessor& field) const {
        return reinterpret_cast<const T*>(&_data[field.offset]);
    }
    template <typename T>
    inline T& get_or_create(const FieldAccessor& field) {
        return *(reinterpret_cast<T*>(&_data[field.offset]));
    }
    // inline Any get_any(const FieldAccessor& field) const {
    //     return field.field_ptr->to_any_func(&_data[field.offset]);
    // }
    inline void clear() noexcept {
        hidden_schema()->clear_all(_data, nullptr);
    }
private:
    inline const Schema* hidden_schema() {
        return hidden_meta().schema;
    }
    inline Schema::Meta& hidden_meta() {
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warray-bounds"
#endif // __clang__
        auto& meta = reinterpret_cast<Schema::Meta*>(_data)[-1];
#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__
        return meta;
    }
    inline const Schema* get_schema() const {
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warray-bounds"
#endif // __clang__
        return (reinterpret_cast<const Schema::Meta*>(_data)[-1]).schema;
#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__
    }
    inline static DynamicStruct* create_with_pool(const Schema&, SerialAllocator<DynamicStruct>);
#ifdef GOOGLE_PROTOBUF_HAS_ARENAS
    inline static DynamicStruct* create_with_pool(const Schema&, ArenaAllocator<DynamicStruct>);
    inline static DynamicStruct* create_with_metadata(
            const Schema&,
            ArenaAllocator<DynamicStruct>,
            const AllocationMetadata&);
#endif
    inline static DynamicStruct* create_with_metadata(
            const Schema&,
            SerialAllocator<DynamicStruct>,
            const AllocationMetadata&);
    char _data[1];
    template <typename A>
    friend class ReuseDynamicStructPool;
};
// SchemaManager 统一管理 名字到Schame 的 映射
class SchemaManager {
public:
    inline static SchemaManager& get_instance() {
        static SchemaManager manager;
        return manager;
    }
    inline int register_schema(const std::string& name, DynamicStruct::Schema* schema) {
        _schema_map.emplace(name, schema);
        return 0;
    }
    inline DynamicStruct::Schema* get_schema(const std::string& name) {
        auto it = _schema_map.find(name);
        if (it != _schema_map.end()) {
            return it->second;
        }
        return nullptr;
    }
private:
    SchemaManager() = default;
    std::unordered_map<std::string, DynamicStruct::Schema*> _schema_map;
};
//定义trait嗅探结构体，是否实现了动态协议
// 结构体注册宏
#define MEMBER_BEGIN        \
    static struct _HIDDEN { \
        ::baidu::feed::mlarch::babylon::DynamicStruct::Schema schema;
#define MEMBER(cls, name)                                        \
    const ::baidu::feed::mlarch::babylon::internal::dynamic_struct::FieldAccessor field_##name = \
            schema.register_field<decltype(name)>(#name, (size_t)(&(((cls*)(0))->name)));
#ifdef GOOGLE_PROTOBUF_HAS_ARENAS
#define ARENA_MEMBER(cls, name)                                  \
    const ::baidu::feed::mlarch::babylon::internal::dynamic_struct::FieldAccessor field_##name = \
            schema.register_field<decltype(name), Arena>(#name, (size_t)(&(((cls*)(0))->name)));
#endif
#define SERIAL_POOL_MEMBER(cls, name)                                \
    const ::baidu::feed::mlarch::babylon::internal::dynamic_struct::FieldAccessor field_##name =     \
            schema.register_field<decltype(name), StaticMemoryPool>( \
                    #name, (size_t)(&(((cls*)(0))->name)));
#define EXCLUSIVE_POOL_MEMBER(cls, name)                                \
    const ::baidu::feed::mlarch::babylon::internal::dynamic_struct::FieldAccessor field_##name =     \
            schema.register_field<decltype(name), ::baidu::feed::mlarch::babylon::ExclusiveMonotonicBufferResource>( \
                    #name, (size_t)(&(((cls*)(0))->name)));
#define SWISS_POOL_MEMBER(cls, name)                                \
    const ::baidu::feed::mlarch::babylon::internal::dynamic_struct::FieldAccessor field_##name =     \
            schema.register_field<decltype(name), ::baidu::feed::mlarch::babylon::SwissMemoryResource>( \
                    #name, (size_t)(&(((cls*)(0))->name)));
#define MEMBER_END(cls)                                                                \
    int hidden = ::baidu::feed::mlarch::babylon::SchemaManager::get_instance().register_schema(#cls, &(this->schema)); \
    int hidden_size = schema.set_size_and_align(sizeof(cls), alignof(cls));\
    }\
    __HIDDEN;\
    static ::baidu::feed::mlarch::babylon::DynamicStruct::Schema& get_struct_schema() { \
        return __HIDDEN.schema;                                                        \
    }                                                                                  
#define REGISTER_STRUCT(cls) cls::_HIDDEN cls::__HIDDEN;
#define FIELD_OF_STRUCT(cls, name) cls::__HIDDEN.schema.get_field(#name);
//定义嗅探是否支持动态结构体协议
}  // namespace babylon
}  // namespace mlarch
}  // namespace feed
}  // namespace baidu
#endif

