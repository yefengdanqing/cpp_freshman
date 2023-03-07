#include<iostream>

#include "google/protobuf/text_format.h"
#include <google/protobuf/util/json_util.h>

#include "person.pb.h"


int main() {
    std::cout << "protobuf start" << std::endl;
    test::Response response;
    response.set_data("www.baidu.com");
    response.set_status(200);
    std::string json_string;
    google::protobuf::util::MessageToJsonString(response, &json_string);
    std::cout << json_string << std::endl;
    return 0;
}