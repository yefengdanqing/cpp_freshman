
#include <string>
#include <iostream>
#include <bthread/bthread.h>
#include <thread>
#include <babylon/reusable/memory_resource.h>


using ::babylon::PageAllocator;
using ::babylon::ExclusiveMonotonicBufferResource;
using ::babylon::SharedMonotonicBufferResource;
using ::babylon::SwissMemoryResource;
using ::babylon::MonotonicBufferResource;


int main() {
    // 默认构造时采用new/delete直接从系统整页申请内存
    // ExclusiveMonotonicBufferResource resource;
    // SharedMonotonicBufferResource resource;
    // SwissMemoryResource resource;

    ExclusiveMonotonicBufferResource resource;
    MonotonicBufferResource& mono_resource = resource;
    mono_resource.allocate(8, 32);
    auto ptr = reinterpret_cast<std::string*>(
        mono_resource.allocate<alignof(::std::string)>(sizeof(std::string)));
    new (ptr)::std::string(1024, 'x');
    std::cout << *ptr << std::endl;
    mono_resource.register_destructor(ptr);




}