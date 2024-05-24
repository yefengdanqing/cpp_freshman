#include "butil/logging.h"
#include "brpc/channel.h"

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


namespace ranker {
namespace ranker {

int32_t ExampleServiceImple::run_main_graph(Graph* graph,
                    const ranker_server::RsRequest* request,
                    ::google::protobuf::Closure *done, 
                    brpc::Controller* cntl,
                    ranker_server::RsResponse* response) {
        auto request_id = graph->find_data("request_id");
        *(request_id->emit<std::string>()) = "6644539be4e6f40001c4d71x";
        auto adx = graph->find_data("adx");
        *(adx->emit<std::string>()) = "mintegral";
        auto time_out = graph->find_data("timeout");
        *(time_out->emit<int32_t>()) = 600;

        
        // 设置响应信息
        response->set_error_msg("Hello " + request->message());
        response->set_request_id(request->message());
}

void ExampleServiceImple::SayHello(google::protobuf::RpcController* cntl_base,
                  const ranker_server::RsRequest* request,
                  ranker_server::RsResponse* response,
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
}
}