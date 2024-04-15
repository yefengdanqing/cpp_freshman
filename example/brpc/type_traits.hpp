#ifndef BAIDU_FEED_MLARCH_BABYLON_LITE_TYPE_TRAITS_HPP
#define BAIDU_FEED_MLARCH_BABYLON_LITE_TYPE_TRAITS_HPP

#include "type_traits.h"

namespace baidu {
namespace feed {
namespace mlarch {
namespace babylon {

///////////////////////////////////////////////////////////////////////////////
// Id begin
#if GLIBCXX_VERSION < 820180726
inline constexpr Id::Id(StringView name) noexcept : name(name) {}
#endif // GLIBCXX_VERSION < 820180726

inline constexpr bool Id::operator==(const Id& other) const noexcept {
    return this == &other;
}

inline constexpr bool Id::operator!=(const Id& other) const noexcept {
    return this != &other;
}

template <typename C, typename T>
inline ::std::basic_ostream<C, T>& operator<<(::std::basic_ostream<C, T>& os,
        const Id& id) noexcept {
    os << id.name;
    return os;
}
// Id end
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// TypeId begin
template <typename T>
inline constexpr StringView TypeId<T>::get_type_name() noexcept {
#define BABYLON_TMP_SV "baidu::feed::mlarch::babylon::StringView"
#define BABYLON_TMP_BSV "baidu::feed::mlarch::babylon::BasicStringView"
    // 从__PRETTY_FUNCTION__表达中截取关键的类型字符串
#ifdef __clang__
    // static std::string_view baidu::feed::mlarch::babylon::TypeId<${type_name}>::get_type_name() [T = ${typename}]
    return StringView(__PRETTY_FUNCTION__ + __builtin_strlen("static " BABYLON_TMP_SV " baidu::feed::mlarch::babylon::TypeId<"),
        (__builtin_strlen(__PRETTY_FUNCTION__) - __builtin_strlen("static " BABYLON_TMP_SV " baidu::feed::mlarch::babylon::TypeId<>::get_type_name() [T = ]")) / 2);
#elif GLIBCXX_VERSION >= 820180726
    // static constexpr std::string_view baidu::feed::mlarch::babylon::TypeId<T>::get_type_name() [with T = ${type_name}; std::string_view = std::basic_string_view<char>]
    return StringView(__PRETTY_FUNCTION__ + __builtin_strlen("static constexpr " BABYLON_TMP_SV " baidu::feed::mlarch::babylon::TypeId<T>::get_type_name() [with T = "),
        __builtin_strlen(__PRETTY_FUNCTION__) - __builtin_strlen("static constexpr " BABYLON_TMP_SV " baidu::feed::mlarch::babylon::TypeId<T>::get_type_name() [with T = ; " BABYLON_TMP_SV " = " BABYLON_TMP_BSV "<char>]"));
#else // !__clang__ && GLIBCXX_VERSION < 820180726
    return StringView();
#endif // !__clang__ && GLIBCXX_VERSION < 820180726

#undef BABYLON_TMP_SV
#undef BABYLON_TMP_BSV
}

template <typename T>
constexpr Id TypeId<T>::ID;
// TypeId end
///////////////////////////////////////////////////////////////////////////////

} // babylon
} // mlarch
} // feed
} // baidu

#endif //BAIDU_FEED_MLARCH_BABYLON_LITE_TYPE_TRAITS_HPP
