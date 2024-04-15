// #ifndef BAIDU_FEED_MLARCH_BABYLON_LITE_TYPE_TRAITS_H
// #define BAIDU_FEED_MLARCH_BABYLON_LITE_TYPE_TRAITS_H

// // #include <baidu/feed/mlarch/babylon/lite/environment.h>
// // #include <baidu/feed/mlarch/babylon/lite/string_view.h>

// #include <atomic>
// #include <memory>
// #include <string_view>


// namespace baidu {
// namespace feed {
// namespace mlarch {
// namespace babylon {

// template <typename T>
// struct TypeId;
// // 表示一种唯一ID
// struct Id {
// #if GLIBCXX_VERSION < 820180726
//     // gcc 4.8.2需要显示定义constexpr构造函数
//     inline constexpr Id(std::string_view name) noexcept;
// #endif
//     // 不能拷贝移动确保唯一性
//     inline Id(Id&& other) = delete;
//     inline Id(const Id& other) = delete;
//     inline Id& operator=(Id&& other) = delete;
//     inline Id& operator=(const Id& other) = delete;

//     // 相等唯一取决于是同一个对象
//     inline constexpr bool operator==(const Id& other) const noexcept;
//     inline constexpr bool operator!=(const Id& other) const noexcept;

//     // ID的人可读明文表达，用于打印日志，不能用于同一性判定
//     const std::string_view name;
    
//     template <typename T>
//     friend struct TypeId;
// };

// // 静态的typeid，不依赖rtti，但是只能判断相等，以及提供人可读的类型名
// // 而没有反射信息，因此也相对会更快
// template <typename T>
// struct TypeId {
//     inline static constexpr std::string_view get_type_name() noexcept;
// #if GLIBCXX_VERSION < 820180726
//     // gcc 4.8.2需要显示使用constexpr构造函数而非列表构造
//     static constexpr Id ID = {get_type_name()};
// #else // GLIBCXX_VERSION >= 820180726
//     static constexpr Id ID = {.name = get_type_name()};
// #endif

// #if !__clang__ && GLIBCXX_VERSION < 920200312
//     // gcc 9.3之后或者clang可以正确处理unused __PRETTY_FUNCTION__
//     // 否则需要引入此trick，通过构造函数引用get_type_name进一步保留 __PRETTY_FUNCTION__
//     // 考虑兼容场景，访问者推荐使用TypeId<T>().ID替换TypeId<T>::ID来使用
//     // 尽管对clang和gcc 9.3来说TypeId<T>::ID可以正确工作
//     inline constexpr TypeId() noexcept : _internal_keep(get_type_name()) {}
// private:
//     const std::string_view _internal_keep;
// #endif
// };

// // // 预期不被使用的类型，用来辅助类型探测
// // struct NeverUsed {
// //     NeverUsed() = delete;
// //     NeverUsed(const NeverUsed&) = delete;
// //     NeverUsed(NeverUsed&&) = delete;
// // };

// // 检测是否F(args)是否可调用
// template <typename F, typename... Args>
// struct IsInvocable {
//     template <typename FF>
//     static auto checker(int32_t) -> decltype(
//         ::std::declval<FF>()(::std::declval<Args>()...));
//     template <typename FF>
//     static NeverUsed checker(...);

//     static constexpr bool value = !::std::is_same<decltype(checker<F>(0)), NeverUsed>::value;
// };

// // 定义一个用于检测是否T::F(args)可调用的检测器
// // F: 希望检测的静态函数名
// // C: 希望生成的检测器名
// // 采用宏实现主要因为函数名F的指定无法通过模版机制实现
// // 用法
// // 1、创建检测器
// // BABYLON_DECLARE_STATIC_INVOCABLE(some_function, Checker);
// // 2、使用检测器检测
// // struct SomeClass {
// //     static void some_function(Arg1, Arg2);
// // };
// // Checker<SomeClass, Arg1, Arg2>::value == true
// // Checker<int32_t, Arg1, Arg2>::value == false
// #define BABYLON_DECLARE_STATIC_INVOCABLE(F, C) \
// template <typename T, typename... Args> \
// struct C { \
//     template <typename U> \
//     static auto checker(int32_t) -> decltype( \
//         U::F(::std::declval<Args>()...)); \
//     template <typename U> \
//     static ::baidu::feed::mlarch::babylon::NeverUsed checker(...); \
//     static constexpr bool value = !::std::is_same<decltype(checker<T>(0)), ::baidu::feed::mlarch::babylon::NeverUsed>::value; \
// };

// // 定义一个用于检测是否T().F(args)可调用的检测器
// // F: 希望检测的成员函数名
// // C: 希望生成的检测器名
// // 采用宏实现主要因为函数名F的指定无法通过模版机制实现
// // 用法
// // 1、创建检测器
// // BABYLON_DECLARE_MEMBER_INVOCABLE(some_function, Checker);
// // 2、使用检测器检测
// // struct SomeClass {
// //     void some_function(Arg1, Arg2);
// // };
// // Checker<SomeClass, Arg1, Arg2>::value == true
// // Checker<int32_t, Arg1, Arg2>::value == false
// #define BABYLON_DECLARE_MEMBER_INVOCABLE(F, C) \
// template <typename T, typename... Args> \
// struct C { \
//     template <typename U> \
//     static auto checker(int32_t) -> decltype( \
//         ::std::declval<U>().F(::std::declval<Args>()...)); \
//     template <typename U> \
//     static ::baidu::feed::mlarch::babylon::NeverUsed checker(...); \
//     static constexpr bool value = !::std::is_same<decltype(checker<T>(0)), ::baidu::feed::mlarch::babylon::NeverUsed>::value; \
// };

// }  // babylon
// }  // mlarch
// }  // feed
// }  // baidu

// #endif //BAIDU_FEED_MLARCH_BABYLON_LITE_TYPE_TRAITS_H

