#include <signal.h>
#include <iostream>
#include <fstream>


#include "butil/logging.h"
#include "brpc/channel.h"
#include "demo_proto.pb.h"
#include "brpc/server.h"
#include "gflags/gflags.h"

#include "babylon/anyflow/builder.h"
#include "babylon/anyflow/graph.h"

#include "common/graph_engine.h"
#include "common/common.h"

using babylon::anyflow::Graph;
using babylon::anyflow::GraphData;
using babylon::anyflow::Closure;
using babylon::anyflow::GraphBuilder;
using babylon::anyflow::GraphProcessor;



DEFINE_int32(idle_timeout_s, -1, "Connection will be closed if there is no read/write operations during the last idle_timeout_s'");
DEFINE_string(stack_trace_file, "./stack_trace.log", "stack trace file");
DEFINE_bool(test_set_flag, false, "test default");
// BRPC_VALIDATE_GFLAG(test_set_flag, brpc::PassValidate/*always true*/);
namespace example {
namespace ranker {


class ParseRequestProcessor : public GraphProcessor {
private:
    //int setup(GraphVertex& vertex) noexcept;
    int process() noexcept override {
        //  *z.emit() = *x + *y;
        RequestInfo tmp = *request;
        tmp.timeout = 100;
        tmp.one_id = "491d1fe44b98ac63991161b21e5bd5af";
        tmp.adx = "integral";
        tmp.price = 1000;
        tmp.request_id = "6646ce74a660e6000108a32x";
        *normalized_request.emit() = std::move(tmp);
    }
    ANYFLOW_INTERFACE (
        ANYFLOW_DEPEND_DATA(RequestInfo, request)
        ANYFLOW_EMIT_DATA(RequestInfo, normalized_request)
    )
};
class PrerankProcessor : public GraphProcessor {
    //int setup(GraphVertex& vertex) noexcept;
private:
    int process() noexcept override {
        //  *z.emit() = *x + *y;
        PrerankInfo info;
        info.ecpm = 99999;
        info.offer_id = 11111;
        info.pivr = 0.02;
        info.request_id = normalized_request->request_id;
        *prerank_info.emit() = std::move(info);
        
    }
    ANYFLOW_INTERFACE (
        ANYFLOW_DEPEND_DATA(RequestInfo, normalized_request)
        ANYFLOW_EMIT_DATA(PrerankInfo, prerank_info)
    )

};
class RankProcessor : public GraphProcessor {
    //int setup(GraphVertex& vertex) noexcept;
private:
    int process() noexcept override {
        //  *z.emit() = *x + *y;
        RankInfo info;
        info.request_id = normalized_request->request_id;
        info.ecpm = 300;
        info.pivr = 0.01 * prerank_info->pivr;
        *rank_info.emit() = std::move(info);
        
    }
    ANYFLOW_INTERFACE (
        ANYFLOW_DEPEND_DATA(RequestInfo, normalized_request)
        ANYFLOW_DEPEND_DATA(PrerankInfo, prerank_info)
        ANYFLOW_EMIT_DATA(RankInfo, rank_info)
    )

};
class OutputProcessor : public GraphProcessor {
    //int setup(GraphVertex& vertex) noexcept;
private:
    int process() noexcept override {
        //  *z.emit() = *x + *y;
        ResponseInfo info;
        info.bid_price = normalized_request->price * prerank_info->pivr * rank_info->pivr * 1000 * prerank_info->ecpm;
        info.request_id = normalized_request->request_id;
        info.timeout = normalized_request->timeout;
        LOG(INFO) << "price:" << normalized_request->price << ",request_id" << info.request_id << ", bid_price:" << info.bid_price << ", pre_pivr:" << prerank_info->pivr << ",rank_pivr" << rank_info->pivr;
        *response_info.emit() = std::move(info);
        
    }
    ANYFLOW_INTERFACE (
        ANYFLOW_DEPEND_DATA(RequestInfo, normalized_request)
        ANYFLOW_DEPEND_DATA(PrerankInfo, prerank_info)
        ANYFLOW_DEPEND_DATA(RankInfo, rank_info)
        ANYFLOW_EMIT_DATA(ResponseInfo, response_info)
    )

};

class PlusProcessor : public GraphProcessor {
private:
    int process() noexcept override {
        *z.emit() = *x + *y;
    }
    // int* x = nullptr;
    // int* y = nullptr;
    // OutputData<int> result;
    ANYFLOW_INTERFACE (
        ANYFLOW_DEPEND_DATA(int, x)
        ANYFLOW_DEPEND_DATA(int, y)
        ANYFLOW_EMIT_DATA(int, z)
    )

};

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
    int32_t run_main_graph(Graph* graph,
                    const example::Request* request,
                    ::google::protobuf::Closure *done, 
                    brpc::Controller* cntl,
                    example::Response* response) {
        auto request_id = graph->find_data("request_id");
        *(request_id->emit<std::string>()) = "6644539be4e6f40001c4d71x";
        auto adx = graph->find_data("adx");
        *(adx->emit<std::string>()) = "mintegral";
        auto time_out = graph->find_data("timeout");
        *(time_out->emit<int32_t>()) = 600;

        
        // 设置响应信息
        response->set_reply("Hello " + request->message());
        response->set_request_id(request->message());
    }
    // 实现SayHello方法
    void SayHello(google::protobuf::RpcController* cntl_base,
                  const example::Request* request,
                  example::Response* response,
                  google::protobuf::Closure* done) {
        brpc::ClosureGuard done_guard(done);
        // 从Controller中获取请求信息
        brpc::Controller* cntl = static_cast<brpc::Controller*>(cntl_base);
        //
        LOG(INFO) << "client_ip:port" << butil::endpoint2str(cntl->remote_side()).c_str();
        LOG(INFO) << "server ip:" << butil::endpoint2str(cntl->local_side());
        if (FLAGS_test_set_flag) {
            LOG(INFO) << "test FLAGS_test_set_flag" << int(FLAGS_test_set_flag);
        }
        {
            GraphBuilder builder;
            builder.set_name("first graph");
            BABYLON_LOG(INFO) << "name:" << builder.name();
            {
                auto& v = builder.add_vertex([]{
                    return std::make_unique<PlusProcessor>();
                });
                v.named_depend("x").to("A");
                v.named_depend("y").to("B");
                v.named_emit("z").to("C");
            }
            builder.finish();
            auto graph = builder.build();
            auto* a = graph->find_data("A");
            auto* b = graph->find_data("B");
            auto* c = graph->find_data("C");
            *(a->emit<int>()) = 100;
            *(b->emit<int>()) = 200;

            graph->run(c);
            std::cout << *c->value<int>() << std::endl;
        }
        {
            GraphBuilder builder_second;
            builder_second.set_name("second_graph");
            BABYLON_LOG(INFO) << "name:" << builder_second.name();
            {
                //方法1
                auto& v0 = builder_second.add_vertex([] {
                    return std::make_unique<ParseRequestProcessor>();
                });
                v0.named_depend("request").to("request");
                v0.named_emit("normalized_request").to("normalized_request");
                //方法2
                auto& v1 = builder_second.add_vertex([] {
                    return std::make_unique<PrerankProcessor>();
                });
                v1.named_depend("normalized_request").to("normalized_request");
                v1.named_emit("prerank_info").to("prerank_info");

                auto& v2 = builder_second.add_vertex([] {
                    return std::make_unique<RankProcessor>();
                });
                v2.named_depend("normalized_request").to("normalized_request");
                v2.named_depend("prerank_info").to("prerank_info");
                v2.named_emit("rank_info").to("rank_info");

                auto& v3 = builder_second.add_vertex([] {
                    return std::make_unique<OutputProcessor>();
                });
                v3.named_depend("normalized_request").to("normalized_request");
                v3.named_depend("prerank_info").to("prerank_info");
                v3.named_depend("rank_info").to("rank_info");
                v3.named_emit("response_info").to("response_info");

                
                builder_second.finish();
                auto graph_second = builder_second.build();
                auto* req = graph_second->find_data("request");
                *(req->emit<RequestInfo>()) = std::move(RequestInfo());
                // auto* prerank_data = graph_second->find_data("prerank_info");
                // auto* rank_data = graph_second->find_data("rank_info");
                auto* output_data = graph_second->find_data("response_info");

                graph_second->run(output_data);
                auto tmp = *output_data->value<ResponseInfo>();
                std::cout << tmp.request_id << " "<< tmp.bid_price << std::endl;

            }


        }

        {

            // graph_engine.init_pool(10, 10, "main_graph");

            //1.通过conf 构建build,比如一些parse配置，如processor、depends、output
            /*processor_name = config_unit[i]["processor"].to_cstr();
            auto processor = context.get<GraphProcessor>(processor_name);
            if (processor == nullptr) {
                LOG(WARNING) << "processor: " << processor_name << " not exists";
                return -1;
            }
            vertex_builder_ptr = &graph_builder.add_vertex((*processor));*/

            //2.
            // auto graph_ptr = graph_engine.get("main_graph");
            // int ret = run_main_graph(graph_ptr, request, done, cntl, response);
        }
        

    }
private:
    GraphEngine graph_engine;


};
}
}


int main() {
    brpc::Server server;
    example::ranker::ExampleServiceImple service_impl;
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
        LOG(ERROR) << "Fail to start EchoServer";
        return -1;
    }
    // 服务启动后，会一直运行，直到调用server.stop(0)停止服务
    server.RunUntilAskedToQuit();
    return 0;
}

