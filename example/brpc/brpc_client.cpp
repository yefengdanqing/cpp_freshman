#include <iostream>
#include "butil/logging.h"
#include "brpc/channel.h"
#include "demo_proto.pb.h"
using google::protobuf::Arena;

int main() {
    brpc::Channel channel;

    // 初始化一个连接
    if (channel.Init("172.20.11.165:8613", nullptr) != 0) {
        LOG(ERROR) << "Fail to initialize channel";
        return -1;
    }
    //通过arena 分配
    Arena arena_request;
    ranker_server::RsRequest* request = Arena::CreateMessage<ranker_server::RsRequest>(&arena_request);

    // Arena arena_response;
    ranker_server::RsResponse* response = Arena::CreateMessage<ranker_server::RsResponse>(&arena_request);

    // 创建 RsRequest 对象
    ranker_server::RsRequest& rs_request = (*request);
    ranker_server::RsResponse& rs_response = (*response);

    // 设置各个字段的值
    rs_request.set_message("Test message");
    rs_request.set_request_id("66503a39b2c54d0007e800dx");
    rs_request.set_adx("skt_test_sdk");
    rs_request.set_one_id("test491d1fe44b98ac63991161b21e5bd5af45");
    rs_request.set_timeout(166);
    rs_request.set_ad_type("reward");
    rs_request.set_channel("trageting");

    // 添加 OfferInfo 对象到 RsRequest 中
    ranker_server::OfferInfo* offer_info = rs_request.add_offer_info();
    offer_info->set_demand_offer_id(123);
    offer_info->set_given_price(10.5);
    offer_info->set_received_price(9.8);
    offer_info->set_given_price_factor(1.2);
    offer_info->set_target_goal_type(1);
    offer_info->set_target_goal_value(100.0);
    offer_info->set_target_goal_window(7);
    offer_info->set_queue_id(1234);
    offer_info->set_index(1);

    // 添加 PackageInfo 对象到 OfferInfo 中
    ranker_server::PackageInfo* package_info = offer_info->mutable_package_info();
    package_info->set_package_name("com.example.app");
    package_info->set_app_name("Example App");
    package_info->set_app_size(1024);
    package_info->set_content_rating(4);
    package_info->set_num_ratings(1000);
    package_info->set_rating(4.5);
    package_info->set_developer_id("developer123");
    package_info->set_bundle_id("com.example.app");
    package_info->set_market_status(1);
    package_info->set_normalized_package_name("com.example.app");
    package_info->set_normalized_developer_id("developer123");
    package_info->set_business_category_1(1);
    package_info->set_business_category_2(2);
    package_info->set_mtg_category_level_1(1);
    package_info->set_mtg_category_level_2(2);
    package_info->set_mtg_category_level_3(3);
    package_info->set_category_type(1);
    package_info->add_market_category("category1");
    package_info->add_market_sub_category("subcategory1");
    package_info->add_similar_packages("similar1");
    package_info->add_developer_packages("developer1");
    

    // 发起RPC调用
    brpc::Controller cntl;
    // 创建一个Stub对象
    ranker_server::ExampleService_Stub stub(&channel);
    stub.SayHello(&cntl, &rs_request, &rs_response, nullptr);

    // 检查调用结果
    if (cntl.Failed()) {
        LOG(ERROR) << "RPC failed: " << cntl.ErrorText();
        return -1;
    }
    //获取server端ip
    LOG(INFO) << "remote_side=" << cntl.remote_side();

    // 输出响应结果
    std::cout << "Reply: " << rs_response.status_code() << " ,msg:" << rs_response.error_msg() << " ,request_id:" << rs_response.request_id() << " ,ext_algo_list:"<< rs_response.ext_algo_list() << std::endl;

    return 0;
}
