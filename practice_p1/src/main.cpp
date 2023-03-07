#include <iostream>

#include <snappy.h>
#include <curl/curl.h>
#include <gtest/gtest.h>


#include "derive.h"


int Foo(int a,int b) {
    if(0 == a||0 == b) {
        throw "don't do that";
    }

    int c = a % b;
    if (0 == c) {
        return b;
    }
    return Foo(b, c);
}

TEST(FooTest, HandleNoneZeroInput) {
   EXPECT_EQ(2, Foo(4,10));
   EXPECT_EQ(6, Foo(30,18));
}


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
void test_curl() {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        std::cout << "http result:"<< readBuffer << std::endl;
  }
}


int a;
const int b = 1;
 
// struct S
// {
//     int x1 : 8 = 42;   // 正确，x1为8位的变量，并且初始化为42，“=42”为常量表达式
//     int x2 : 6 {42};   // 正确，x2为6位的变量，并且初始化为42
//     int x3 : true ? 10 : a = 20; // 正确，x3为10位变量，不进行初始化，赋值号优先于三目运算符
//     //int x4 : true ? 10 : b = 20; // 错误，b为const变量，不可赋值
//     int x5 : (true ? 10 : b) = 20; // 正确，x5为10位的变量，并且初始化为20
//     //int x6 : false ? 10 : a = 20; // 错误，a = 10不是常量表达式
// };



// int test_snappy() {
//     std::string input = "Hello World";
//     std::string output;
//     for (int i = 0; i < 5; ++i) {
//         input += input;
//     }
//     snappy::Compress(input.data(), input.size(), &output);
//     std::cout << "input size:" << input.size() << " output size:"
//        << output.size() << std::endl;
//     std::string output_uncom;
//     snappy::Uncompress(output.data(), output.size(), &output_uncom);
//     if (input == output_uncom) {
//         std::cout << "Equal" << std::endl;
//     } else {
//         std::cout << "ERROR: not equal" << std::endl;
//     }
// }

int main() {
    practice_p1::Base base;
    base.print();
    practice_p1::Derive derive;
    derive.print1();
    // std::cout << s.x1 << std::endl;
    int a[] = {1, 2, 3, 4};
    test_curl();
    //test_snappy();
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();


}