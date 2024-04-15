#include <signal.h>
#include <iostream>
#include <fstream>


#include "butil/logging.h"
#include "brpc/channel.h"
#include "demo_proto.pb.h"
#include "brpc/server.h"
#include "gflags/gflags.h"




DEFINE_int32(idle_timeout_s, -1, "Connection will be closed if there is no read/write operations during the last idle_timeout_s'");
DEFINE_string(stack_trace_file, "./stack_trace.log", "stack trace file");


// 对捕获的信号做处理
static void server_core_signal_handler(int signum) {
    std::ofstream out(FLAGS_stack_trace_file.c_str(), std::ios::app);
    // base::debug::StackTrace trace;
    out << std::endl << "Program terminated with signal " << signum << std::endl;
    // trace.OutputToStream(&out);

    signal(signum, SIG_DFL); // stack_trace输出结束后，再绑回默认的handler继续处理
    raise(signum);
}

// 注册需要捕获的core信号
void install_single_handler() {
    signal(SIGSEGV, server_core_signal_handler);
    signal(SIGILL, server_core_signal_handler);
    signal(SIGABRT, server_core_signal_handler);
    signal(SIGFPE, server_core_signal_handler);
    signal(SIGBUS, server_core_signal_handler);
}

// 实现一个继承自ExampleService的服务类
class ExampleServiceImple : public example::ExampleService {
    public:
    // 实现SayHello方法
    void SayHello(google::protobuf::RpcController* cntl_base,
                  const example::Request* request,
                  example::Response* response,
                  google::protobuf::Closure* done) {
        // 从Controller中获取请求信息
        brpc::Controller* cntl = static_cast<brpc::Controller*>(cntl_base);
        // 设置响应信息
        response->set_reply("Hello " + request->message());
        response->set_request_id(request->message());
        // 调用done->Run()表示响应已经完成
        done->Run();
    }

};
int main() {
    brpc::Server server;
    ExampleServiceImple service_impl;
    // 注册服务
    if (server.AddService(&service_impl,
                          brpc::SERVER_DOESNT_OWN_SERVICE) != 0) {
        LOG(ERROR) << "Fail to add service";
        return -1;
    }
    brpc::ServerOptions options;
    options.idle_timeout_sec = FLAGS_idle_timeout_s;
    // options.enabled_protocols = "baidu_std_reuse";
    // 设置监听地址
    if (server.Start(12345, &options) != 0) {
        LOG(ERROR) << "Fail to start EchoServer";
        return -1;
    }
    // 服务启动后，会一直运行，直到调用server.stop(0)停止服务
    server.RunUntilAskedToQuit();
    return 0;
}

