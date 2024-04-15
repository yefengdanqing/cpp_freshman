
// #ifndef BAIDU_FEED_MLARCH_BABYLON_DYNAMIC_STRUCT_HPP
// #define BAIDU_FEED_MLARCH_BABYLON_DYNAMIC_STRUCT_HPP
// #include "ynamic_struct.h"


// // #include <baidu/feed/mlarch/babylon/memory_pool.h>

// namespace babylon {
// namespace internal {

// namespace dynamic_struct {
// template <
//         typename T,
//         typename A,
//         typename std::enable_if<
//                 std::is_pointer<T>::value &&
//                         is_dynamic_structible<typename std::remove_cv<
//                                 typename std::remove_pointer<T>::type>::type>::value,
//                 int>::type>
// inline const FieldAccessor BaseSchema::register_field(const std::string& field_name, size_t offset) {
//     typedef typename std::remove_cv<typename std::remove_pointer<T>::type>::type real_type;
//     return register_field<T, A>(
//             field_name, FieldType::DYNAMIC_STRUCT_POINTER, offset, &real_type::get_struct_schema());
// }
// // T 是指针类型，*T不支持动态结构体协议
// template <
//         typename T,
//         typename A,
//         typename std::enable_if<
//                 std::is_pointer<T>::value &&
//                         !is_dynamic_structible<typename std::remove_cv<
//                                 typename std::remove_pointer<T>::type>::type>::value,
//                 int>::type>
// inline const FieldAccessor BaseSchema::register_field(const std::string& field_name, size_t offset) {
//     return register_field<T, A>(field_name, FieldType::OTHER, offset, nullptr);
// }
// // T 不是指针类型，T支持动态结构体协议
// template <
//         typename T,
//         typename A,
//         typename std::
//                 enable_if<!std::is_pointer<T>::value && is_dynamic_structible<T>::value, int>::type>
// inline const FieldAccessor BaseSchema::register_field(const std::string& field_name, size_t offset) {
//     return register_field<T, A>(
//             field_name, FieldType::DYNAMIC_STRUCT, offset, &T::get_struct_schema());
// }
// // T 不是指针类型，T不支持动态结构体协议
// template <
//         typename T,
//         typename A,
//         typename std::enable_if<
//                 !std::is_pointer<T>::value && !is_dynamic_structible<T>::value,
//                 int>::type>
// inline const FieldAccessor BaseSchema::register_field(const std::string& field_name, size_t offset) {
//     return register_field<T, A>(field_name, FieldType::OTHER, offset, nullptr);
// }
// template <typename T, typename A>
// inline const FieldAccessor BaseSchema::register_field(
//         const std::string& field_name,
//         FieldType field_type,
//         size_t offset,
//         BaseSchema* schema) noexcept {
//     auto it = _field_map.find(field_name);
//     if (it != _field_map.end()) {
//         return FieldAccessor(it->second);
//     }
//     std::lock_guard<std::mutex> guard(_mutex);
//     Field field;
//     if (offset == std::string::npos) {
//         field = Field(
//                 add_field_and_caculate_size<T>(), field_name, &TypeId<T>().ID, field_type, schema, to_any<T>);
//     } else {
//         field = Field(offset, field_name, &TypeId<T>().ID, field_type, schema, to_any<T>);
//     }
//     add_constructor<T, A>(field);
//     add_destructor<T, A>(field);
//     add_cleaner<T, A>(field);
//     add_meta_data<T, A>();
//     add_update_metadata<T, A>(field);
//     add_contruct_with_metadata<T, A>(field);
//     _field_map.emplace(field.name, field);
//     return FieldAccessor(_field_map[field.name]);
// }
// inline void BaseSchema::destroy_all(char* ptr) const noexcept {
//     for (auto& destructor : _destructors) {
//         ::std::get<1>(destructor)(ptr + ::std::get<0>(destructor));
//     }
// }
// inline void BaseSchema::construct_all(char* ptr, void* pool = nullptr) const noexcept {
//     for (auto& constructor : _constructors) {
//         ::std::get<2>(constructor)(
//                 ptr + ::std::get<0>(constructor), pool, nullptr, ::std::get<1>(constructor));
//     }
// }
// inline void BaseSchema::construct_with_metadata_all(
//         char* ptr,
//         void* pool,
//         const std::vector<void*>& metadatas) const noexcept {
//     size_t i = 0;
//     for (auto& constructor : _with_meta_constructor) {
//         ::std::get<2>(constructor)(
//                 ptr + ::std::get<0>(constructor), pool, metadatas[i], ::std::get<1>(constructor));
//         ++i;
//     }
// }
// inline void BaseSchema::clear_all(char* ptr, void* pool) const noexcept {
//     for (auto& cleaner : _cleaners) {
//         std::get<2>(cleaner)(ptr + std::get<0>(cleaner), pool, nullptr, ::std::get<1>(cleaner));
//     }
// }
// inline size_t BaseSchema::update_metadata(const char* ptr, std::vector<void*> metadata) const
//         noexcept {
//     size_t size = 0;
//     for (size_t i = 0; i < _metadata_updator.size(); ++i) {
//         if (std::get<1>(_metadata_updator[i]) && metadata[i]) {
//             size += std::get<1>(_metadata_updator[i])(
//                     ptr + std::get<0>(_metadata_updator[i]), metadata[i]);
//         }
//     }
//     return size;
// }
// inline const FieldAccessor BaseSchema::get_field(const std::string& field_name) const {
//     auto it = _field_map.find(field_name);
//     if (it != _field_map.end()) {
//         return FieldAccessor(it->second);
//     } else {
//         abort();
//     }
// }

// inline const FieldAccessor BaseSchema::get_field(const std::string& field_name, int& err) const {
//     auto it = _field_map.find(field_name);
//     if (it != _field_map.end()) {
//         err = 0;
//         return FieldAccessor(it->second);
//     } else {
//         err = -1;
//         return FieldAccessor(Field());
//     }
// }

// inline const std::unordered_map<std::string, Field> *BaseSchema::get_all_fields() const {
//     return &_field_map;
// }

// template <typename T, typename A, typename std::enable_if<!std::is_void<A>::value, int>::type>
// inline void BaseSchema::add_constructor(const Field& field) {
//     if (::std::is_trivial<T>::value) {
//         if (_constructors.empty() || ::std::get<2>(_constructors.back()) != fill_with_zero) {
//             _constructors.emplace_back(field.offset, sizeof(T), fill_with_zero);
//         } else {
//             auto& last_zero_constructor = _constructors.back();
//             ::std::get<1>(last_zero_constructor) =
//                     field.offset - ::std::get<0>(last_zero_constructor) + sizeof(T);
//         }
//     } else {
//         _constructors.emplace_back(field.offset, sizeof(T), construct_with_pool<T, A>);
//     }
// }
// template <typename T, typename A, typename std::enable_if<std::is_void<A>::value, int>::type>
// inline void BaseSchema::add_constructor(const Field& field) {
//     if (::std::is_trivial<T>::value) {
//         if (_constructors.empty() || ::std::get<2>(_constructors.back()) != fill_with_zero) {
//             _constructors.emplace_back(field.offset, sizeof(T), fill_with_zero);
//         } else {
//             auto& last_zero_constructor = _constructors.back();
//             ::std::get<1>(last_zero_constructor) =
//                     field.offset - ::std::get<0>(last_zero_constructor) + sizeof(T);
//         }
//     } else {
//         _constructors.emplace_back(field.offset, sizeof(T), construct<T>);
//     }
// }
// template <typename T, typename A, typename std::enable_if<!std::is_trivially_destructible<T>::value && std::is_void<A>::value, int>::type>
// inline void BaseSchema::add_destructor(const Field& field) {
//         _destructors.emplace_back(field.offset, destroy<T>);
//         _is_trivially_destructible &= std::is_trivially_destructible<T>::value;
// }

// template <typename T, typename A, typename std::enable_if<std::is_trivially_destructible<T>::value || !std::is_void<A>::value, int>::type>
// inline void BaseSchema::add_destructor(const Field&) {
//         _is_trivially_destructible &= std::is_trivially_destructible<T>::value;
// }

// template <typename T, typename A>
// inline void BaseSchema::add_meta_data() {
//     if (ReusableTraits<
//                 T,
//                 MonotonicAllocator<
//                         T,
//                         typename std::conditional<std::is_void<A>::value, StaticMemoryPool, A>::
//                                 type>>::use_metadata()) {
//         _metadata_creator.emplace_back(create_meta<T, A>);
//         _metadata_deletor.emplace_back(delete_meta<T, A>);
//     } else {
//         _metadata_creator.emplace_back(nullptr);
//         _metadata_deletor.emplace_back(nullptr);
//     }
// }
// template <typename T, typename A>
// inline void BaseSchema::add_update_metadata(const Field& field) {
//     if (ReusableTraits<
//                 T,
//                 MonotonicAllocator<
//                         T,
//                         typename std::conditional<std::is_void<A>::value, StaticMemoryPool, A>::
//                                 type>>::use_metadata()) {
//         _metadata_updator.emplace_back(field.offset, update_metadata<T, A>);
//     } else {
//         _metadata_updator.emplace_back(field.offset, nullptr);
//     }
// }
// template <typename T, typename A>
// inline void BaseSchema::add_contruct_with_metadata(const Field& field) {
//     if (::std::is_trivial<T>::value) {
//         if (_with_meta_constructor.empty() ||
//             ::std::get<2>(_with_meta_constructor.back()) != fill_with_zero) {
//             _with_meta_constructor.emplace_back(field.offset, sizeof(T), fill_with_zero);
//         } else {
//             auto& last_zero_constructor = _with_meta_constructor.back();
//             ::std::get<1>(last_zero_constructor) =
//                     field.offset - ::std::get<0>(last_zero_constructor) + sizeof(T);
//         }
//     } else {
//         _with_meta_constructor.emplace_back(field.offset, sizeof(T), construct_with_metadata<T, A>);
//     }
// }
// template <typename T, typename A, typename std::enable_if<std::is_trivial<T>::value, int>::type>
// inline void BaseSchema::add_cleaner(const Field& field) {
//     if (_cleaners.empty() || ::std::get<2>(_cleaners.back()) != fill_with_zero) {
//         _cleaners.emplace_back(field.offset, sizeof(T), fill_with_zero);
//     } else {
//         auto& last_zero_cleaner = _cleaners.back();
//         ::std::get<1>(last_zero_cleaner) =
//                 field.offset - ::std::get<0>(last_zero_cleaner) + sizeof(T);
//     }
// }
// template <typename T, typename A, typename std::enable_if<!std::is_trivial<T>::value, int>::type>
// inline void BaseSchema::add_cleaner(const Field& field) {
//     _cleaners.emplace_back(field.offset, sizeof(T), reconstruct<T, A>);
// }
// }  // namespace dynamic_struct
// }  // namespace internal
// //////////////////////////////////DynamicStrucut implement/////////////////////////
// inline std::unique_ptr<DynamicStruct> DynamicStruct::create(const Schema& schema) {
// #if __cpp_aligned_new
//     const Schema** ptr = reinterpret_cast<const Schema**>(
//             ::operator new(schema.allocate_size(), std::align_val_t(schema.allocate_align())));
// #else
//     const Schema** ptr = reinterpret_cast<const Schema**>(
//             ::aligned_alloc(schema.allocate_align(), schema.allocate_size()));
// #endif
//     *ptr = &schema;
//     auto ds = reinterpret_cast<DynamicStruct*>(ptr + 1);
//     ds->hidden_schema()->construct_all(ds->_data);
//     return ::std::unique_ptr<DynamicStruct>(ds);
// }
// template <typename A, typename ::std::enable_if<::std::is_constructible<std::allocator<DynamicStruct, A>, A*>::value, int32_t>::type>
// inline DynamicStruct* DynamicStruct::create_with_pool(
//         const Schema& schema,
//         A* resource) {
//     auto allocator = std::allocator<DynamicStruct, A>(resource);
//     const Schema** ptr = reinterpret_cast<const Schema**>(allocator.allocate_bytes(schema.allocate_size(),
//                 schema.allocate_align()));
//     *ptr = &schema;
//     auto ds = reinterpret_cast<DynamicStruct*>(ptr + 1);
//     char* data_ptr = reinterpret_cast<char*>(&(ds->_data));
//     ds->hidden_schema()->construct_all(data_ptr, resource);
//     allocator.register_destructor(ds);
//     return ds;
// }
// inline DynamicStruct* DynamicStruct::get_dynamic_struct(const FieldAccessor& field) {
//     DynamicStruct* ds = nullptr;
//     if (field.field_type == FieldType::DYNAMIC_STRUCT) {
//         ds = get<DynamicStruct>(field);
//     } else if (field.field_type == FieldType::DYNAMIC_STRUCT_POINTER) {
//         ds = *(get<DynamicStruct*>(field));
//     }
//     return ds;
// }
// inline const DynamicStruct* DynamicStruct::cget_dynamic_struct(const FieldAccessor& field) const {
//     const DynamicStruct* ds = nullptr;
//     if (field.field_type == FieldType::DYNAMIC_STRUCT) {
//         ds = cget<DynamicStruct>(field);
//     } else if (field.field_type == FieldType::DYNAMIC_STRUCT_POINTER) {
//         ds = *(cget<DynamicStruct*>(field));
//     }
//     return ds;
// }
// inline DynamicStruct* DynamicStruct::create_with_pool(
//         const Schema& schema,
//         SerialAllocator<DynamicStruct> allocator) {
//     auto pool = allocator.underlying;
//     const Schema** ptr = reinterpret_cast<const Schema**>(
//             pool->create_aligned_buffer(schema.allocate_size(), schema.allocate_align()));
//     *ptr = &schema;
//     auto ds = reinterpret_cast<DynamicStruct*>(ptr + 1);
//     char* data_ptr = reinterpret_cast<char*>(&(ds->_data));
//     ds->hidden_schema()->construct_all(data_ptr, pool);
//     return ds;
// }
// #ifdef GOOGLE_PROTOBUF_HAS_ARENAS
// inline DynamicStruct* DynamicStruct::create_with_pool(
//         const Schema& schema,
//         ArenaAllocator<DynamicStruct> allocator) {
//     auto pool = allocator.underlying;
//     const Schema** ptr = reinterpret_cast<const Schema**>(
//             Arena::CreateArray<char>(pool, schema.allocate_size()));
//     *ptr = &schema;
//     DynamicStruct* ds = reinterpret_cast<DynamicStruct*>(ptr + 1);
//     char* data_ptr = reinterpret_cast<char*>(&(ds->_data));
//     ds->hidden_schema()->construct_all(data_ptr, pool);
//     return ds;
// }
// inline DynamicStruct* DynamicStruct::create_with_metadata(
//         const Schema&,
//         ArenaAllocator<DynamicStruct> allocator,
//         const AllocationMetadata& metadata) {
//     auto pool = allocator.underlying;
//     auto schema = metadata.schema;
//     const Schema** ptr = reinterpret_cast<const Schema**>(
//             Arena::CreateArray<char>(pool, schema->allocate_size()));
//     *ptr = schema;
//     auto ds = reinterpret_cast<DynamicStruct*>(ptr + 1);
//     ds->hidden_schema()->construct_with_metadata_all(ds->_data, pool, metadata.metadatas);
//     return ds;
// }
// #endif
// inline DynamicStruct* DynamicStruct::create_with_metadata(
//         const Schema&,
//         std::allocator<DynamicStruct> allocator,
//         const AllocationMetadata& metadata) {
//     auto pool = allocator.underlying;
//     auto schema = metadata.schema;
//     const Schema** ptr = reinterpret_cast<const Schema**>(
//             pool->create_aligned_buffer(schema->allocate_size(), schema->allocate_align()));
//     *ptr = schema;
//     auto ds = reinterpret_cast<DynamicStruct*>(ptr + 1);
//     ds->hidden_schema()->construct_with_metadata_all(ds->_data, pool, metadata.metadatas);
//     return ds;
// }
// //////////////////////////////////ReuseDynamicStructPool implement/////////////////////////
// template <typename A>
// inline size_t ReuseDynamicStructPool<A>::update_allocation_metadata(
//         AllocationMetadata& pool_metadata) const {
//     auto& metadata = pool_metadata.ds_metadata;
//     if (metadata.schema == nullptr) {
//         metadata.metadatas.reserve(_schema->get_field_size());
//         for (auto& create_meta_cb : _schema->get_create_meta()) {
//             if (create_meta_cb) {
//                 void* create_meta_ptr = create_meta_cb();
//                 metadata.metadatas.push_back(create_meta_ptr);
//             } else {
//                 metadata.metadatas.push_back(nullptr);
//             }
//         }
//         metadata.schema = _schema;
//     }
//     size_t size = 0;
//     for (auto& ds : _ds_vec) {
//         size += metadata.schema->update_metadata(ds->_data, metadata.metadatas);
//     }
//     pool_metadata.size = _ds_vec.size();
//     return size;
// }
// template <typename A>
// inline ReuseDynamicStructPool<A>::ReuseDynamicStructPool(
//         ::std::allocator_arg_t,
//         allocator_type allocator,
//         const AllocationMetadata& metadata) :
//         _ds_vec(std::allocator_arg, std::allocator<DynamicStruct*, A>(*allocator.underlying)) {
//     _ds_vec.reserve(metadata.size);
//     auto& ds_metadata = metadata.ds_metadata;
//     for (size_t i = 0; i < metadata.size; ++i) {
//         _ds_vec.push_back(DynamicStruct::create_with_metadata(
//                 *(ds_metadata.schema),
//                 MonotonicAllocator<DynamicStruct, A>(*allocator.underlying),
//                 ds_metadata));
//     }
//     use_num = 0;
//     _pool = allocator.underlying;
//     _schema = ds_metadata.schema;
// }
// template <typename A>
// inline void ReuseDynamicStructPool<A>::reconstruct() {
//     for (size_t i = 0; i < use_num; ++i) {
//         _schema->clear_all(_ds_vec[i]->_data, _pool);
//     }
//     use_num = 0;
// }
// template <typename A>
// inline DynamicStruct* ReuseDynamicStructPool<A>::get() {
//     DynamicStruct* ds = nullptr;
//     if (use_num == _ds_vec.size()) {
//         ds = DynamicStruct::create_with_pool(*_schema, *_pool);
//         _ds_vec.push_back(ds);
//         ++use_num;
//         return ds;
//     } else {
//         ds = _ds_vec[use_num];
//         ++use_num;
//     }
//     return ds;
// }
// }  // namespace babylon

// #endif
