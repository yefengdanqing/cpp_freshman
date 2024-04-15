#ifndef BAIDU_FEED_MLARCH_BABYLON_ANY_HPP
#define BAIDU_FEED_MLARCH_BABYLON_ANY_HPP

#include "any.h"
#include "helper.h"

namespace baidu {
namespace feed {
namespace mlarch {
namespace babylon {

///////////////////////////////////////////////////////////////////////////////
// Any::TypeDescriptor begin
template <typename T>
constexpr Any::Descriptor Any::TypeDescriptor<T, typename ::std::enable_if<
    ::std::is_copy_constructible<T>::value>::type>::descriptor;
template <typename T>
constexpr Any::Descriptor Any::TypeDescriptor<T, typename ::std::enable_if<
    !::std::is_copy_constructible<T>::value>::type>::descriptor;

template <typename T, typename E>
void Any::TypeDescriptor<T, E>::destructor(void* object) noexcept {
    reinterpret_cast<T*>(object)->~T();
}

template <typename T, typename E>
void Any::TypeDescriptor<T, E>::deleter(void* object) noexcept {
    delete reinterpret_cast<T*>(object);
}

template <typename T>
void Any::TypeDescriptor<T, typename ::std::enable_if<
        ::std::is_copy_constructible<T>::value>::type>::copy_constructor(
            void* ptr, const void* object) {
    new (ptr) T(*reinterpret_cast<const T*>(object));
}

template <typename T>
void Any::TypeDescriptor<T, typename ::std::enable_if<
        !::std::is_copy_constructible<T>::value>::type>::copy_constructor(
            void*, const void*) {
    // 不支持拷贝，反馈运行时失败
    LOG_EVERY_SECOND(FATAL) << "try copy non-copyable instance by copy an babylon::Any instance";
}

template <typename T>
void* Any::TypeDescriptor<T, typename ::std::enable_if<
        ::std::is_copy_constructible<T>::value>::type>::copy_creater(
            const void* object) {
    return new T(*reinterpret_cast<const T*>(object));
}

template <typename T>
void* Any::TypeDescriptor<T, typename ::std::enable_if<
        !::std::is_copy_constructible<T>::value>::type>::copy_creater(
            const void*) {
    // 不支持拷贝，反馈运行时失败
    LOG_EVERY_SECOND(FATAL) << "try copy non-copyable instance by copy an babylon::Any instance";
    return nullptr;
}
// Any::TypeDescriptor end
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Any begin
inline Any::Any() noexcept :
    _meta {.v = 0},
    _holder {.pointer_value = nullptr} {}

inline Any::Any(const Any& other) :
        _meta(other._meta) {
    switch (_meta.m.holder_type) {
    case HolderType::INPLACE_NON_TRIVIAL: {
            _meta.descriptor()->copy_constructor(&_holder, &other._holder);
            break;
        }
    case HolderType::INSTANCE: {
            _holder.pointer_value = _meta.descriptor()->copy_creater(other._holder.pointer_value);
            break;
        }
    default: {
            _holder = other._holder;
            break;
        }
    }
}

inline Any::Any(Any& other) : Any(const_cast<const Any&>(other)) {}

inline Any::Any(Any&& other) noexcept :
        _meta(other._meta),
        _holder(other._holder) {
    new (&other) Any;
}

template<typename T>
inline Any::Any(::std::unique_ptr<T>&& value) noexcept :
    _meta {.v = reinterpret_cast<uint64_t>(&TypeDescriptor<T>().descriptor) |
        static_cast<uint64_t>(HolderType::INSTANCE) << 56 | static_cast<uint64_t>(Type::INSTANCE) << 48},
    _holder {.pointer_value = value.release()} {}

template <typename T, typename ::std::enable_if<sizeof(size_t) < sizeof(T), int32_t>::type>
inline Any::Any(T&& value) :
    _meta {.v = reinterpret_cast<uint64_t>(&TypeDescriptor<typename ::std::decay<T>::type>().descriptor) |
        static_cast<uint64_t>(HolderType::INSTANCE) << 56 | static_cast<uint64_t>(Type::INSTANCE) << 48},
    _holder {.pointer_value = new typename ::std::decay<T>::type(::std::forward<T>(value))} {}

template <typename T, typename ::std::enable_if<sizeof(T) <= sizeof(size_t), int32_t>::type>
inline Any::Any(T&& value) :
        _meta {.v = reinterpret_cast<uint64_t>(&TypeDescriptor<typename ::std::decay<T>::type>().descriptor)} {
    // 无法填满holder的实例进行预填充，避免后续报操作未初始化值
    if (sizeof(T) < sizeof(size_t)) {
        _holder.pointer_value = nullptr;
    }
    new (&_holder) typename ::std::decay<T>::type(::std::forward<T>(value));
    _meta.m.holder_type = ::std::is_trivial<T>::value ? HolderType::INPLACE_TRIVIAL : HolderType::INPLACE_NON_TRIVIAL;
    _meta.m.type = Type::INSTANCE;
}

#define BABYLON_TMP_SPECIALIZE_FOR_PRIMITIVE(ctype, etype, field) \
inline Any::Any(ctype value) noexcept : \
    _meta {.v = reinterpret_cast<uint64_t>(&TypeDescriptor<ctype>().descriptor) | \
        static_cast<uint64_t>(HolderType::INPLACE_TRIVIAL) << 56 | static_cast<uint64_t>(Type::etype) << 48}, \
    _holder {.field = value} {} \
\
inline Any::Any(::std::unique_ptr<ctype>&& value) noexcept : \
    _meta {.v = reinterpret_cast<uint64_t>(&TypeDescriptor<ctype>().descriptor) | \
        static_cast<uint64_t>(HolderType::INSTANCE) << 56 | static_cast<uint64_t>(Type::etype) << 48}, \
    _holder {.pointer_value = value.release()} {} \
\
template <> \
inline Any& Any::cref<ctype>(const ctype& value) noexcept { \
    destroy(); \
    _meta.v = reinterpret_cast<uint64_t>(&TypeDescriptor<ctype>().descriptor) | \
        static_cast<uint64_t>(HolderType::CONST_REFERENCE) << 56 | static_cast<uint64_t>(Type::etype) << 48; \
    _holder.const_pointer_value = &value; \
    return *this; \
}\
template <> \
inline Any& Any::ref<ctype>(ctype& value) noexcept { \
    destroy(); \
    _meta.v = reinterpret_cast<uint64_t>(&TypeDescriptor<ctype>().descriptor) | \
        static_cast<uint64_t>(HolderType::MUTABLE_REFERENCE) << 56 | static_cast<uint64_t>(Type::etype) << 48; \
    _holder.pointer_value = &value; \
    return *this; \
}

BABYLON_TMP_SPECIALIZE_FOR_PRIMITIVE(int64_t, INT64, int64_v);
BABYLON_TMP_SPECIALIZE_FOR_PRIMITIVE(int32_t, INT32, int64_v);
BABYLON_TMP_SPECIALIZE_FOR_PRIMITIVE(int16_t, INT16, int64_v);
BABYLON_TMP_SPECIALIZE_FOR_PRIMITIVE(int8_t, INT8, int64_v);
BABYLON_TMP_SPECIALIZE_FOR_PRIMITIVE(uint64_t, UINT64, uint64_v);
BABYLON_TMP_SPECIALIZE_FOR_PRIMITIVE(uint32_t, UINT32, uint64_v);
BABYLON_TMP_SPECIALIZE_FOR_PRIMITIVE(uint16_t, UINT16, uint64_v);
BABYLON_TMP_SPECIALIZE_FOR_PRIMITIVE(uint8_t, UINT8, uint64_v);
BABYLON_TMP_SPECIALIZE_FOR_PRIMITIVE(double, DOUBLE, double_v);
BABYLON_TMP_SPECIALIZE_FOR_PRIMITIVE(float, FLOAT, float_v);
BABYLON_TMP_SPECIALIZE_FOR_PRIMITIVE(bool, BOOLEAN, uint64_v);

#undef BABYLON_TMP_SPECIALIZE_FOR_PRIMITIVE
    
inline Any::~Any() noexcept {
    destroy();
}

template <typename T>
inline Any& Any::operator=(T&& other) {
    alignas(Any) char any_data[sizeof(Any)];
    void* ptr = any_data;
    new (ptr) Any(::std::forward<T>(other));
    destroy();
    _meta = reinterpret_cast<Any*>(ptr)->_meta;
    _holder = reinterpret_cast<Any*>(ptr)->_holder;
    return *this;
}

inline Any& Any::operator=(const Any& other) {
    return this->operator=<const Any&>(other);
}

template <typename T>
inline Any& Any::ref(T& value) noexcept {
    destroy();
    _meta.v = reinterpret_cast<uint64_t>(&TypeDescriptor<T>().descriptor) |
        static_cast<uint64_t>(HolderType::MUTABLE_REFERENCE) << 56 | static_cast<uint64_t>(Type::INSTANCE) << 48;
    _holder.pointer_value = &value;
    return *this;
}

template <typename T>
inline Any& Any::cref(const T& value) noexcept {
    destroy();
    _meta.v = reinterpret_cast<uint64_t>(&TypeDescriptor<T>().descriptor) |
        static_cast<uint64_t>(HolderType::CONST_REFERENCE) << 56 | static_cast<uint64_t>(Type::INSTANCE) << 48;
    _holder.const_pointer_value = &value;
    return *this;
}

template <>
inline Any& Any::ref<Any>(Any& other) noexcept {
    destroy();
    _meta.v = other._meta.v | (static_cast<uint64_t>(HolderType::MUTABLE_REFERENCE) << 56);
    _holder.pointer_value = other.raw_pointer();
    return *this;
}

template <>
inline Any& Any::cref<Any>(const Any& other) noexcept {
    destroy();
    _meta.v = other._meta.v | (static_cast<uint64_t>(HolderType::CONST_REFERENCE) << 56);
    _holder.const_pointer_value = other.const_raw_pointer();
    return *this;
}

template <typename T>
inline Any& Any::ref(const T& value) noexcept {
    return cref(value);
}

inline void Any::clear() noexcept {
    destroy();
    new (this) Any;
}

template<typename T>
inline T* Any::get() noexcept {
    if (!is_const_reference()) {
        return const_cast<T*>(cget<T>());
    }
    return nullptr;
}

template <typename T, typename ::std::enable_if<sizeof(size_t) < sizeof(T), int32_t>::type>
inline const T* Any::cget() const noexcept {
    if (_meta.m.descriptor32 == static_cast<uint32_t>(reinterpret_cast<uint64_t>(&TypeDescriptor<T>().descriptor))) {
        return reinterpret_cast<const T*>(_holder.pointer_value);
    }
    return nullptr;
}

template <typename T, typename ::std::enable_if<sizeof(T) <= sizeof(size_t), int32_t>::type>
inline const T* Any::cget() const noexcept {
    if (_meta.m.descriptor32 == static_cast<uint32_t>(reinterpret_cast<uint64_t>(&TypeDescriptor<T>().descriptor))) {
        return reinterpret_cast<const T*>(const_raw_pointer());
    }
    return nullptr;
}

template<typename T>
inline const T* Any::get() const noexcept {
    return cget<T>();
}

inline bool Any::is_const_reference() const noexcept {
    return _meta.v & (static_cast<uint64_t>(HolderType::CONST) << 56);
}

inline bool Any::is_reference() const noexcept {
    return _meta.v & (static_cast<uint64_t>(HolderType::REFERENCE) << 56);
}

template<typename T>
inline T Any::as() const noexcept {
    switch (_meta.m.type) {
        case Type::INT64:
            return *reinterpret_cast<const int64_t*>(const_raw_pointer());
        case Type::INT32:
            return *reinterpret_cast<const int32_t*>(const_raw_pointer());
        case Type::INT16:
            return *reinterpret_cast<const int16_t*>(const_raw_pointer());
        case Type::INT8:
            return *reinterpret_cast<const int8_t*>(const_raw_pointer());
        case Type::BOOLEAN:
            return *reinterpret_cast<const bool*>(const_raw_pointer());
        case Type::UINT64:
            return *reinterpret_cast<const uint64_t*>(const_raw_pointer());
        case Type::UINT32:
            return *reinterpret_cast<const uint32_t*>(const_raw_pointer());
        case Type::UINT16:
            return *reinterpret_cast<const uint16_t*>(const_raw_pointer());
        case Type::UINT8:
            return *reinterpret_cast<const uint8_t*>(const_raw_pointer());
        case Type::DOUBLE:
            return *reinterpret_cast<const double*>(const_raw_pointer());
        case Type::FLOAT:
            return *reinterpret_cast<const float*>(const_raw_pointer());
        default:
            return 0;
    }
}

template<>
inline int32_t Any::to<int64_t>(int64_t& value) const noexcept {
    if (_meta.m.type > Type::INSTANCE) {
        value = as<int64_t>();
        return 0;
    }
    return -1;
}

inline Any::operator bool() const noexcept {
    return _meta.m.type != Type::EMPTY;
}

inline Any::Type Any::type() const noexcept {
    return _meta.m.type;
}

inline const Id& Any::instance_type() const noexcept {
    return _meta.descriptor()->type_id;
}

inline void Any::destroy() noexcept {
    switch (_meta.m.holder_type) {
    case HolderType::INSTANCE: {
            _meta.descriptor()->deleter(_holder.pointer_value);
            break;
        }
    case HolderType::INPLACE_NON_TRIVIAL: {
            _meta.descriptor()->destructor(&_holder);
            break;
        }
    default: {
            break;
        }
    }
}

inline void* Any::raw_pointer() noexcept {
    return _meta.m.holder_type < HolderType::INSTANCE ? 
        &_holder : _holder.pointer_value;
}

inline const void* Any::const_raw_pointer() const noexcept {
    return _meta.m.holder_type < HolderType::INSTANCE ? 
        &_holder : _holder.const_pointer_value;
}
// Any end
///////////////////////////////////////////////////////////////////////////////

} // babylon
} // mlarch
} // feed
} // baidu

#endif // BAIDU_FEED_MLARCH_BABYLON_ANY_HPP
