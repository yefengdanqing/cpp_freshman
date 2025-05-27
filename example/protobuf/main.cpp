#include<iostream>
#include<atomic>
#include <thread>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <string>

#include "google/protobuf/text_format.h"
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/message.h>

#include "person.pb.h"
// #include "memory_pool.h"

// #include "type_traits.h"
// #include "environment.h"

// 定义字段序列
#define PERSON_FIELDS \
    ((std::string, data)) \
    ((int32_t, status))

// 定义结构体和注册字段
#define DEFINE_FIELD(r, _, FIELD) \
    BOOST_PP_TUPLE_ELEM(2, 0, FIELD) BOOST_PP_TUPLE_ELEM(2, 1, FIELD);

#define DEFINE_STRUCT(NAME, FIELDS) \
    struct NAME { \
        BOOST_PP_SEQ_FOR_EACH(DEFINE_FIELD, _, FIELDS) \
    };

DEFINE_STRUCT(Person, PERSON_FIELDS)

// struct <- proto
#define ASSIGN_FIELD_STRING(r, _, FIELD) \
{ \
    const std::string& field_name = BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 1, FIELD)); \
    const auto* field_desc = desc->FindFieldByName(field_name); \
    if (field_desc && field_desc->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_STRING) { \
        s.BOOST_PP_TUPLE_ELEM(2, 1, FIELD) = refl->GetString(proto, field_desc); \
    } \
}

// 针对 int32_t 字段
#define ASSIGN_FIELD_INT32(r, _, FIELD) \
{ \
    const std::string& field_name = BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 1, FIELD)); \
    const auto* field_desc = desc->FindFieldByName(field_name); \
    if (field_desc && field_desc->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_INT32) { \
        s.BOOST_PP_TUPLE_ELEM(2, 1, FIELD) = refl->GetInt32(proto, field_desc); \
    } \
}
template <typename StructT>
void ProtoToStruct(const google::protobuf::Message& proto, StructT& s) {
    const auto* desc = proto.GetDescriptor();
    const auto* refl = proto.GetReflection();
    // 分类型处理
    // BOOST_PP_SEQ_FOR_EACH(ASSIGN_FIELD_STRING, _, PERSON_FIELDS)
    // BOOST_PP_SEQ_FOR_EACH(ASSIGN_FIELD_INT32, _, PERSON_FIELDS)
}



// // proto <- struct
// 针对 std::string 字段




// // 定义字段序列 (name, type)
// #define FIELDS \
//     ((data, std::string)) \
//     ((status, int32_t)) 

// // 生成 struct
// #define DEFINE_STRUCT(NAME, FIELDS) \
// struct NAME { \
//     BOOST_PP_SEQ_FOR_EACH(DEFINE_FIELD, _, FIELDS) \
// };

// // 定义单个字段
// #define DEFINE_FIELD(r, _, FIELD) \
//     BOOST_PP_TUPLE_ELEM(2, 1, FIELD) BOOST_PP_TUPLE_ELEM(2, 0, FIELD);

// // 生成 Person 结构体
// DEFINE_STRUCT(Person, FIELDS)


// // 将 protobuf 消息赋值到 struct
// template <typename StructT>
// void ProtoToStruct(const google::protobuf::Message& proto, StructT& s) {
//     const auto* desc = proto.GetDescriptor();
//     const auto* refl = proto.GetReflection();

//     #define ASSIGN_FIELD(r, _, PERSON_FIELDS) \
//     { \
//         const std::string& field_name = BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 0, PERSON_FIELDS)); \
//         const auto* field_desc = desc->FindFieldByName(field_name); \
//         if (field_desc) { \
//             if (field_desc->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_STRING) { \
//                 s.BOOST_PP_TUPLE_ELEM(2, 0, PERSON_FIELDS) = refl->GetString(proto, field_desc); \
//             } else if (field_desc->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_INT32) { \
//                 s.BOOST_PP_TUPLE_ELEM(2, 0, PERSON_FIELDS) = refl->GetInt32(proto, field_desc); \
//             } else if (field_desc->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_FLOAT) { \
//                 s.BOOST_PP_TUPLE_ELEM(2, 0, PERSON_FIELDS) = refl->GetFloat(proto, field_desc); \
//             } \
//         } \
//     }

//     BOOST_PP_SEQ_FOR_EACH(ASSIGN_FIELD, _, PERSON_FIELDS)
// }

// // 将 struct 赋值到 protobuf 消息
// template <typename StructT>
// void StructToProto(const StructT& s, google::protobuf::Message& proto) {
//     const auto* desc = proto.GetDescriptor();
//     const auto* refl = proto.GetReflection();

//     #define SET_FIELD(r, _, PERSON_FIELDS) \
//     { \
//         const std::string& field_name = BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 0, PERSON_FIELDS)); \
//         const auto* field_desc = desc->FindFieldByName(field_name); \
//         if (field_desc) { \
//             if (field_desc->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_STRING) { \
//                 refl->SetString(&proto, field_desc, s.BOOST_PP_TUPLE_ELEM(2, 0, PERSON_FIELDS)); \
//             } else if (field_desc->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_INT32) { \
//                 refl->SetInt32(&proto, field_desc, s.BOOST_PP_TUPLE_ELEM(2, 0, PERSON_FIELDS)); \
//             } else if (field_desc->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_FLOAT) { \
//                 refl->SetFloat(&proto, field_desc, s.BOOST_PP_TUPLE_ELEM(2, 0, PERSON_FIELDS)); \
//             } \
//         } \
//     }

//     BOOST_PP_SEQ_FOR_EACH(SET_FIELD, _, PERSON_FIELDS)
// }





// // 定义一个宏：如果条件为 1，选择 "Yes"，否则选择 "No"
// #define YES_OR_NO(COND) BOOST_PP_IIF(COND, "Yes", "No")
// // 宏定义：展开成变量定义
// #define DECLARE_VAR(r, data, elem) \
//     BOOST_PP_TUPLE_ELEM(2, 0, elem) BOOST_PP_TUPLE_ELEM(2, 1, elem);



bool test_reflection() {
    std::cout << BOOST_PP_STRINGIZE(YES_OR_NO(1)) << std::endl;  // 输出 "Yes"
    std::cout << BOOST_PP_STRINGIZE(YES_OR_NO(0)) << std::endl;  // 输出 "No"

    // BOOST_PP_SEQ_FOR_EACH(DECLARE_VAR, _, PERSON_FIELDS)

    // 1. 生成 Person 结构体
    Person p;

    // 2. 从 protobuf 赋值到 struct
    test::Response proto;
    proto.set_data("Alice");
    proto.set_status(25);

    ProtoToStruct(proto, p);
    std::cout << "Name: " << p.data << ", Age: " << p.status << std::endl;

    // 3. 从 struct 赋值到 protobuf
    Person p2;
    p2.data = "Bob";
    p2.status = 30;

    test::Response proto2;
    // StructToProto(p2, proto2);
    std::cout << "Proto Name: " << proto2.data() << ", Age: " << proto2.status() << std::endl;

    return 0;
}
int testxxxxxx() {
    std::cout << "protobuf start" << std::endl;
    test::Response response;
    response.set_data("www.baidu.com");
    response.set_status(200);
    std::string json_string;
    google::protobuf::util::MessageToJsonString(response, &json_string);
    std::cout << json_string << std::endl;
}
int main() {
    std::atomic<int> sum(0);
    std::thread t1([&sum]{
        for(int i = 0; i < 10000; i++) {
            sum.fetch_add(1, std::memory_order_relaxed);
        }
    });
    
    std::thread t2([&sum]{
        for(int i = 0; i < 10000; i++) {
            sum.fetch_add(1, std::memory_order_relaxed);
        }
    });
    t2.join();
    t1.join();

    std::cout << "xxxxxxx:" << sum.load() << std::endl;
    test_reflection();
    return 0;
}
