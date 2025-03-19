

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"

#include "babylon/anyflow/builtin/expression.h"
#include "babylon/application_context.h"
#include "babylon/any.h"
#include "gflags/gflags.h"
#include "babylon/reusable/page_allocator.h"



#include "graph_engine.h"
#include "bthread_graph_executor.h"


using babylon::anyflow::builtin::ExpressionProcessor;
using babylon::anyflow::GraphData;
using babylon::anyflow::Closure;
using babylon::anyflow::GraphBuilder;
using babylon::anyflow::GraphProcessor;
using babylon::anyflow::GraphVertexBuilder;
using babylon::anyflow::GraphDependencyBuilder;
using babylon::anyflow::GraphEmitBuilder;
using ::babylon::ApplicationContext;
using ::babylon::Any;

namespace utopian {
namespace framework {

DEFINE_uint64(framework_max_free_page_num, 8192, "framework_free_page_num");
DEFINE_uint64(framework_page_size, 4096, "framework_page_size");

std::unique_ptr<GraphExecutor> GraphEngine::_executor = nullptr;
::std::unique_ptr<PageHeap> GraphEngine::_s_page_heap = std::unique_ptr<PageHeap>(new PageHeap());





Graph* GraphEngine::try_get(const std::string& name) noexcept {
    auto iter = _graphs.find(name);
    if (iter == _graphs.end()) {
        return nullptr;
    }
    return iter->second->pop().get();

}
std::unique_ptr<Graph>& GraphEngine::create() noexcept {
    return _graph;
}

void GraphEngine::check_executor() {
    if (GraphEngine::_executor == nullptr) {
        GraphEngine::_executor = ::std::unique_ptr<ThreadPoolGraphExecutor>(new ThreadPoolGraphExecutor);
        //后续通过bthread执行
        ThreadPoolGraphExecutor* specific_executor = dynamic_cast<ThreadPoolGraphExecutor*>(_executor.get());
        if (specific_executor) {
            int ret = specific_executor->initialize(20, 20);  // 调用派生类的实现
            if (ret == -1) {
                BABYLON_LOG(INFO) << "ThreadPoolGraphExecutor initialize failed";
            }
        } else {
            LOG(INFO) << "not use special executor";
            // 处理转换失败的情况
        }
        // executor.initialize(thread_num, queue_capacity);
        // 投入使用
        // 等待已提交任务排空，并关闭执行线程
        // executor.stop();
    }
}

int GraphEngine::initialize() noexcept {
    check_executor();
    return 0;
}

//可以从配置文件加载

bool GraphEngine::init_pool(int size, int cache_size, const std::string& name) noexcept {
    {
        _s_page_heap.reset(new PageHeap(FLAGS_framework_max_free_page_num, FLAGS_framework_page_size));
    }
    if (_graphs.find(name) == _graphs.end()) {
        auto iter = _graphs.emplace(name, std::make_unique<GraphPool>());
        if (iter.second == true) {
            BABYLON_LOG(INFO) << "first grap pool set";
        } else {
            BABYLON_LOG(INFO) << "grap pool set";
        }
        auto& one_graph_pool = _graphs[name];
        one_graph_pool->reserve_and_clear(size);
        
        // Use bthread to run graph processor
        builder.set_executor(BthreadGraphExecutor::instance());

        //use page
        // babylon::PageAllocator *page_allocator = _s_page_heap.get();
        // builder.set_page_allocator(*page_allocator);

        boost::property_tree::ptree pt;
        std::ifstream xml_file("config.xml"); 
        boost::property_tree::read_xml(xml_file, pt);
        // 遍历每个 vertex和每个表达式,后续可以再加一个配置文件，支持表达式,可以用一个函数解决
        for (const auto& vertex : pt.get_child("graph.vertices")) {
            LOG(INFO) << "Vertex:";
            std::string processor_name = vertex.second.get<std::string>("processor");
            //这里可以用工厂模式或者其他方式VertexUnitParser::parse
            BABYLON_LOG(INFO) << "  Processor: " << processor_name;
            GraphVertexBuilder* vertex_builder_ptr = nullptr;
            auto accessor = ApplicationContext::instance().component_accessor<GraphProcessor>(processor_name);
            // auto option = vertex_node["option"];
            // auto& vertex = _builder.add_vertex([accessor, option]() mutable {
            //     ::babylon::Any option_any {option};
            //         return ::std::unique_ptr<GraphProcessor> {
            //         accessor.create(option_any).release()};
            //     });

            //为啥是mutable
            vertex_builder_ptr = &builder.add_vertex([accessor]() mutable {
                return ::std::unique_ptr<GraphProcessor> {accessor.create().release()};
            });

            // 遍历每个 dependency、outputs、condition
            for (const auto& dependency : vertex.second.get_child("dependencies")) {
                const auto& depend_str = dependency.second.data();
                vertex_builder_ptr->named_depend(depend_str).to(depend_str);
            }
            for (const auto& output : vertex.second.get_child("outputs")) {
                const auto& output_str = output.second.data();
                vertex_builder_ptr->named_emit(output_str).to(output_str);
            }
        }
        //在解析配置之后
        //表达式
        pt.clear();
        std::ifstream exp_xml_file("expression.xml");
        boost::property_tree::read_xml(exp_xml_file, pt);
        for (const auto& expression : pt.get_child("expressions")) {
            const std::string& exp_name = expression.second.get<std::string>("name");
            const std::string& exp_val = expression.second.get<std::string>("value");
            LOG(INFO) << "name:" << exp_name << ", value:" << exp_val;
            ExpressionProcessor::apply(builder, exp_name, exp_val);
        }
        ExpressionProcessor::apply(builder);

        // auto page_heap_plugin = context.get<PageHeapPlugin>();
        // builder.page_heap(&page_heap_plugin->page_heap());

        // 表达式应用需要在finish之前，之后正常使用graph
        //完成图的构建
        int ret = builder.finish();
        if (ret == 0) {
            LOG(INFO) << "finish success:0";
        } else {
            LOG(INFO) << "finish success:" << ret;
        }


        one_graph_pool->set_creator([this]{
            auto graph = builder.build().release();
            return graph;
        });
        one_graph_pool->set_recycler([](Graph& graph) {
            graph.reset();
        });
    

    LOG(TRACE) << "init graph pool success. pool size:" << size 
               << ", cached size:" << cache_size
               << ", reserve global:" << size;
    }
    return true;
    


    // pool->set_creator(
    //     [thist]{
    //         auto graph = builder.build().release();
    //         return graph;
    //     }
    // );
    // _graphs[name] = pool.release();
    // parallel loop:
    // 通过pop获取实例，如果对象池为空，会自动调用构造回调
    //auto ptr = pool.pop();
    //ptr->...; // 返回值为定制Deleter的智能指针
    // 析构时实例自动归还，池内实例超出容量N后，超出部分会直接销毁
    
}

BABYLON_REGISTER_COMPONENT(GraphEngine, "graph_engine");
}
}