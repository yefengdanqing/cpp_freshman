#include "graph_engine.h"
namespace utopian {
namespace ranker {

Graph* GraphEngine::get(const std::string& name) noexcept {
    // auto it = _graphs.find(name);
    // if (it != _graphs.end()) {
    //     return it->second.get();
    // }
    return nullptr;
}


Graph GraphEngine::try_get(const std::string& name) noexcept {



}
std::unique_ptr<Graph> GraphEngine::create() noexcept {

}
void GraphEngine::executor(GraphExecutor* executor) {

}
bool GraphEngine::init_pool(int size, int cache_size, const std::string& name) noexcept {
    auto pool = std::unique_ptr<GraphPool>(new GraphPool());
    if (pool == nullptr) {
        BABYLON_LOG(INFO) << "set pool failed";
        return false;
    }
    pool->reserve_and_clear(size);
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
}
}