#include <signal.h>
#include <iostream>
#include <fstream>

#include "gflags/gflags.h"
#include "service/ranker_service.h"
#include "common/global.h"


DEFINE_int32(idle_timeout_s, -1, "Connection will be closed if there is no read/write operations during the last idle_timeout_s'");
DEFINE_string(stack_trace_file, "./stack_trace.log", "stack trace file");
// BRPC_VALIDATE_GFLAG(test_set_flag, brpc::PassValidate/*always true*/);


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

int main(int argc, char* argv[]) {
    //处理参数变量
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::SetVersionString("v1");
    google::SetCommandLineOption("flagfile", "./conf/gflags.conf");

    //brpc 监控
    // brpc最多dump 65535个纬度的监控
    google::SetCommandLineOption("bvar_max_dump_multi_dimension_metric_number", "65535");
    install_single_handler();

    //初始化全局的变量
    CHECK_RET_EXIT(utopian::ranker::GlobalInitializer::get_global_instance().init(), "global init failed,exit");



    brpc::Server server;
    utopian::ranker::ExampleServiceImple service_impl;
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
    butil::EndPoint ep;
    str2endpoint("172.20.11.165:8613", &ep);
    if (server.Start(ep, &options) != 0) {
        LOG(ERROR) << "Fail to start say server";
        return -1;
    }
    // 服务启动后，会一直运行，直到调用server.stop(0)停止服务
    server.RunUntilAskedToQuit();
    _exit(0);
    return 0;
}

