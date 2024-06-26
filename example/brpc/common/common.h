#pragma once
#include <iostream>
#include "butil/logging.h"

#define DEFINE_ERROR_CODE(err_name, no) static int err_name = no;

#define CHECK_RET_EXIT(ret, s)\
    if (ret != 0) { \
        LOG(FATAL) << s; \
        exit(1); \
    }
#define CHECK_PTR_NULL(ptr, ...) \
    if (ptr == nullptr || ptr == NULL) { \
        LOG(WARNING) << #ptr << "is null"; \
        return __VA_ARGS__; \
    } \

#define CHECT_RET(ret, val, s, args...) \
    if (ret != val) {                \
        LOG(WARNING) << s;           \
        return args;                 \
    }                                \

namespace utopian{
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
struct RoasFactor {
    double ratio = 1.0;
    double beta = 1.0;
    double theta = 0.0;
    double a = 0.2;
    double b = 0.001;
    double c = 0.9;
};

}
}