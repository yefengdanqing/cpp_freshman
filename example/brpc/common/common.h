#pragma once
#include <iostream>

#define CHECT_RET_EXIT(ret, n)\
    if (ret != 0) { \
        std::cout << "error: " << n << std::endl; \
        exit(1); \
    }

namespace example{
namespace  ranker {
struct RequestInfo {
    int32_t timeout;
    double price = 0.0;
    std::string request_id;
    std::string adx;
    std::string one_id;
};

struct PrerankInfo {
    int32_t queue_id = 0;
    int32_t index = -1;
    int64_t offer_id = 0;
    double pivr = 0.0;
    double ecpm = 0.0;
    std::string request_id;
};
struct RankInfo {
    int32_t index = -1;
    double pivr = 0.0;
    double ecpm = 0.0;

    std::string request_id;

};
struct ResponseInfo {
    int32_t timeout;
    double bid_price = 0.0;
    double bid_floor = 0.0;

    std::string request_id;
};

}
}