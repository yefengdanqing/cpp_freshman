#include <fstream>

#include "butil/logging.h"
#include "brpc/channel.h"

// #include "fmt/format.h"

#include "babylon/application_context.h"

#include "boost/property_tree/ptree.hpp"
#include <boost/property_tree/xml_parser.hpp>

#include "ranker_service.h"
#include "processor/parse_request.h"
#include "processor/prerank.h"
#include "processor/ranker.h"
#include "processor/output.h"






DEFINE_bool(test_set_flag, false, "test default");

using babylon::anyflow::GraphData;
using babylon::anyflow::Closure;
using babylon::anyflow::GraphBuilder;
using babylon::anyflow::GraphProcessor;
using babylon::anyflow::GraphVertexBuilder;
using babylon::ApplicationContext;
using ranker_server::RsRequest;
using ranker_server::RsResponse;



namespace utopian {
namespace ranker {

int32_t ExampleServiceImple::run_main_graph(Graph* graph,
                    const RsRequest* request,
                    ::google::protobuf::Closure *done, 
                    brpc::Controller* cntl,
                    RsResponse* response) {

        
}

void ExampleServiceImple::SayHello(google::protobuf::RpcController* cntl_base,
                  const RsRequest* request,
                  RsResponse* response,
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
    if (request == nullptr || response == nullptr) {
        LOG(INFO) << "req or response is null";
    } else {
        LOG(INFO) << "req and response is not null,id:" << request->request_id();
    }
    
    {
        auto graph_engine = ApplicationContext::instance().get<framework::GraphEngine>("graph_engine");
        auto graph = graph_engine->try_get("main_graph");
        // auto& graph = graph_engine->create();
        if (graph == nullptr) {
            LOG(WARNING) << "graph engine is null";
        }

        //非只读
        auto* req = graph->find_data("request");
        *(req->emit<RsRequest>()) = *request;
        // 只读,可以preset
        const auto output_data = graph->find_data("rs_response");
        // 【高级用法】在图运行之前，将some_exist_instance引用预置到data内部
        // 后续data->emit<T>()时会使用some_exist_instance作为底层实例
        // 主要用于优化向外部（比如通信框架层）的数据传输，通过允许图直接操作外部实例，来达到避免拷贝的目的
        output_data->preset<RsResponse>(*response);
        Closure closure = graph->run(output_data);
        // auto tmp = *output_data->value<ResponseInfo>();
        // std::cout << tmp.request_id << " "<< tmp.bid_price << std::endl;
        //是否可以直接用指针呢，内存怎么处理的
        const auto tmp = output_data->value<RsResponse>();
        BABYLON_LOG(INFO) << "result:" <<tmp->request_id() << ", response:" << response->request_id();
        // 设置响应信息，根据状态码填充信息
        // response->CopyFrom(*(tmp));
        // 重置图的运行状态，以便重复使用同一个Graph实例
        // 重置之前需要彻底结束上一次运行，即等待之前的closure彻底结束（析构或者closure.wait）
        closure.wait();
        //~closure;自动调用wait
        bool finished = closure.finished();
        auto error_code = closure.get();
        LOG(INFO) << error_code;
        graph->reset();
        //有个response的问题？
    }
    // {
    //     const std::string& ref = fmt::format("Name: {}", "Alice"); // 悬空引用
    //     LOG(INFO) << ref; // 未定义行为
    // }
    
}



}
}