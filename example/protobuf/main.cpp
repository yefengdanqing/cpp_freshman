#include<iostream>
#include<atomic>
#include <thread>

#include "google/protobuf/text_format.h"
#include <google/protobuf/util/json_util.h>

#include "person.pb.h"
// #include "memory_pool.h"

// #include "type_traits.h"
// #include "environment.h"



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
    return 0;
}