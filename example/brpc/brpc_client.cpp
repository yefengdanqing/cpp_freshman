#include <iostream>
#include "butil/logging.h"
#include "brpc/channel.h"
#include "demo_proto.pb.h"

int main() {
    brpc::Channel channel;

    // 初始化一个连接
    if (channel.Init("localhost:12345", nullptr) != 0) {
        LOG(ERROR) << "Fail to initialize channel";
        return -1;
    }

    

    // 创建请求和响应对象
    example::Request request;
    example::Response response;

    // 设置请求参数
    request.set_message("World");
    request.set_request_id("12345678x");
    

    // 发起RPC调用
    brpc::Controller cntl;
    // 创建一个Stub对象
    example::ExampleService_Stub stub(&channel);
    stub.SayHello(&cntl, &request, &response, nullptr);

    // 检查调用结果
    if (cntl.Failed()) {
        LOG(ERROR) << "RPC failed: " << cntl.ErrorText();
        return -1;
    }
    //获取server端ip
    LOG(INFO) << "remote_side=" << cntl.remote_side();

    // 输出响应结果
    std::cout << "Reply: " << response.reply() << " " << response.request_id()<< std::endl;

    return 0;
}
