// #include<iostream>


// #include "butil/logging.h"
// #include "person.pb.h"
// // #include "dynamic_struct.h"
// #include "helper.h"

// using ::baidu::feed::mlarch::babylon;

// // struct S {
// //     int f1;
// //     float f2;
// //     MEMBER_BEGIN
// //     MEMBER(S, f1)
// //     MEMBER(S, f2)
// //     MEMBER_END(S)
// // };
// // REGISTER_STRUCT(S)
// // struct MyStruct {
// //     int f1{1000};
// //     std::string f2;
// //     S f3;
// //     MEMBER_BEGIN
// //     MEMBER(MyStruct, f1)
// //     MEMBER(MyStruct, f2)
// //     MEMBER(MyStruct, f3)
// //     MEMBER_END(MyStruct)
// // };
// // REGISTER_STRUCT(MyStruct)


// int main() {
//     /*{
//         auto f1_field = FIELD_OF_STRUCT(MyStruct, f1);
//         auto f2_field = FIELD_OF_STRUCT(MyStruct, f2);
//         auto f3_field = FIELD_OF_STRUCT(MyStruct, f3);
    
//         MyStruct s;
//         s.f1 = -1024;
//         s.f2 = "hello";
//         s.f3.f1 = 1024;
//         auto ds = DynamicStruct::struct_to_dynamic(&s);
//         std::cout << "f1: " << *(ds->get<int>(f1_field)) << std::endl;
//         std::cout << "f2: " << *(ds->get<std::string>(f2_field)) << std::endl;
//         std::cout << "f3.f1: " << (ds->get<S>(f3_field))->f1 << std::endl;
//     }*/
//     {
//         struct S {
//             using allocator_type = ::std::allocator<S>;
//             S(::std::allocator_arg_t, allocator_type, const ::std::string&) {}
//             S(::std::allocator_arg_t, allocator_type, ::std::string&& s) {
//                 s.clear();
//             }
//         };

//         ::std::string value = "10086";
//         AllocatorConstructHelper::construct((S*)1024, ::std::allocator<void>(), "10086");

//         if((AllocatorConstructHelper::Constructible<S, ::std::allocator<void>, const char*>::USES_ALLOCATOR)) {
//             std::cout << "USES_ALLOCATOR" << std::endl;
//         } else {
//             std::cout << "NOT USES_ALLOCATOR" << std::endl;
//         }

//         AllocatorConstructHelper::construct((S*)1024, ::std::allocator<void>(), value);
//         if((AllocatorConstructHelper::Constructible<S, ::std::allocator<void>, ::std::string&>::USES_ALLOCATOR)) {
//             std::cout << "USES_ALLOCATOR" << std::endl;
//         } else {
//             std::cout << "NOT USES_ALLOCATOR" << std::endl;
//         }
//         std::cout << value.empty() << " " << value << std::endl;

//         AllocatorConstructHelper::construct((S*)1024, ::std::allocator<void>(), ::std::move(value));
//         if((AllocatorConstructHelper::Constructible<S, ::std::allocator<void>, ::std::string>::USES_ALLOCATOR)) {
//             std::cout << "USES_ALLOCATOR" << std::endl;
//         } else {
//             std::cout << "NOT USES_ALLOCATOR" << std::endl;
//         }

//         if ((AllocatorConstructHelper::Constructible<S, ::std::allocator<void>, ::std::string&&>::USES_ALLOCATOR)) {
//             std::cout << "USES_ALLOCATOR" << std::endl;
//         } else {
//             std::cout << "NOT USES_ALLOCATOR" << std::endl;
//         }
//         std::cout << value.empty() << " " << value << std::endl;
//     }
//     return 0;
// }