#pragma once

#include "demo_proto.pb.h"
#include "brpc/server.h"

#include "framework/graph_engine.h"


namespace utopian {
namespace ranker {
// 实现一个继承自ExampleService的服务类
class ExampleServiceImple : public ranker_server::ExampleService {
    public:
    int32_t run_main_graph(Graph* graph,
                    const ranker_server::RsRequest* request,
                    ::google::protobuf::Closure *done, 
                    brpc::Controller* cntl,
                    ranker_server::RsResponse* response);
    // 实现SayHello方法
    void SayHello(google::protobuf::RpcController* cntl_base,
                  const ranker_server::RsRequest* request,
                  ranker_server::RsResponse* response,
                  google::protobuf::Closure* done);
private:
    framework::GraphEngine graph_engine;
};

}
}