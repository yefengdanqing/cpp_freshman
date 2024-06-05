#include <fstream>

#include "butil/logging.h"
#include "brpc/channel.h"

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
        LOG(INFO) << "req and response is not null";
    }

    {
        GraphBuilder builder_second;
        builder_second.set_name("second_graph");
        BABYLON_LOG(INFO) << "name:" << builder_second.name();
        {
            boost::property_tree::ptree pt;
            std::ifstream xml_file("config.xml"); 
            boost::property_tree::read_xml(xml_file, pt);
            // 遍历每个 vertex
            for (const auto& vertex : pt.get_child("graph.vertices")) {
                LOG(INFO) << "Vertex:";
                const auto& process = vertex.second.get<std::string>("processor");
                //这里可以用工厂模式或者其他方式VertexUnitParser::parse
                GraphVertexBuilder* vertex_builder_ptr = nullptr;
                {
                    if (process == "ParseRequestProcessor") {
                        vertex_builder_ptr = &builder_second.add_vertex([] {
                            return std::make_unique<ParseRequestProcessor>();
                        });
                    } else if (process == "RankProcessor") {
                        vertex_builder_ptr = &builder_second.add_vertex([] {
                            return std::make_unique<RankProcessor>();
                        });
                    } else if (process == "OutputProcessor") {
                        vertex_builder_ptr = &builder_second.add_vertex([] {
                            return std::make_unique<OutputProcessor>();
                        });
                    } else if (process == "PrerankProcessor") {
                        vertex_builder_ptr = &builder_second.add_vertex([] {
                            return std::make_unique<PrerankProcessor>();
                        });
                    } else {
                        continue;
                    }
                }
                BABYLON_LOG(INFO) << "  Processor: " << process;
                // 遍历每个 dependency、outputs、condition、expression
                for (const auto& dependency : vertex.second.get_child("dependencies")) {
                    const auto& depend_str = dependency.second.data();
                    vertex_builder_ptr->named_depend(depend_str).to(depend_str);
                }
                for (const auto& output : vertex.second.get_child("outputs")) {
                    const auto& output_str = output.second.data();
                    vertex_builder_ptr->named_emit(output_str).to(output_str);
                }
            }
            int ret = builder_second.finish();
            if (ret == 0) {
                LOG(INFO) << "finish success:0";
            } else {
                LOG(INFO) << "finish success:" << ret;
            }

            auto graph_second = builder_second.build();
            //非只读
            auto* req = graph_second->find_data("request");
            *(req->emit<RsRequest>()) = *request;
            // 只读
            const auto output_data = graph_second->find_data("rs_response");
            // 【高级用法】在图运行之前，将some_exist_instance引用预置到data内部
            // 后续data->emit<T>()时会使用some_exist_instance作为底层实例
            // 主要用于优化向外部（比如通信框架层）的数据传输，通过允许图直接操作外部实例，来达到避免拷贝的目的
            output_data->preset<RsResponse>(*response);

            Closure closure = graph_second->run(output_data);
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
            if (finished) {
                LOG(INFO) << "true";
            } else {
                LOG(INFO) << "False";
            }
            auto error_code = closure.get();
            LOG(INFO) << error_code;
            graph_second->reset();
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