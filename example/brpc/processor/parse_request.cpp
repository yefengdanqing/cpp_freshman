#include "babylon/application_context.h"


#include "parse_request.h"
#include "common/common.h"



using ::babylon::ApplicationContext;

namespace utopian {
namespace ranker {

int ParseRequestProcessor::process() noexcept {
    BABYLON_LOG(INFO) << "ParseRequestProcessor::process";
    RequestInfo tmp;
        
    tmp.timeout = request->timeout();
    tmp.one_id = request->one_id();
    tmp.adx = request->adx();
    tmp.price = 1000;
    tmp.request_id = request->request_id();
    // auto tmp_config = *option<RoasFactor>();
    // BABYLON_LOG(INFO) << "ratio:" << tmp_config.ratio;
    *normalized_request.emit() = std::move(tmp);
    *IsSdkTraffic.emit() = request->adx() == "skt_test_sdkx";
    *IsNotIvrv.emit() = !(request->ad_type() == "reward" || request->adx() == "inist");
    const std::string channel = request->channel();
    BABYLON_LOG(INFO) << "channel:" << channel;
    *IsSpecialChannel.emit() = (request->channel() == "sdk_ee" || request->channel() == "target");
     return 0;

}

// int ParseRequestProcessor::config(const Any& origin_option, Any& option) const noexcept {
//     BABYLON_LOG(INFO) << "ParseRequestProcessor::config";
//     RoasFactor tmp;
//     tmp.ratio = 10;
//     tmp.beta = 10;
//     tmp.theta = 10;
//     tmp.a = 10;
//     tmp.b = 10;
//     tmp.c = 10;
//     const RoasFactor* conf = origin_option.get<RoasFactor>();
//     if (conf != nullptr) {
//         tmp.ratio += conf->ratio;
//         option = std::move(tmp);
//     }
//     return 0;
// }
// int ParseRequestProcessor::setup() noexcept {
//     const RoasFactor* tmp = option<RoasFactor>();
//     if (tmp != nullptr) {
//         BABYLON_LOG(INFO) << "ParseRequestProcessor::setup:" << tmp->ratio;
//     }
//     BABYLON_LOG(INFO) << "ParseRequestProcessor::setup";
//     return 0;
// }
   
// BABYLON_REGISTER_COMPONENT(ParseRequestProcessor, "ParseRequestProcessor", GraphProcessor);
BABYLON_REGISTER_FACTORY_COMPONENT(ParseRequestProcessor, "ParseRequestProcessor", GraphProcessor);
}
}