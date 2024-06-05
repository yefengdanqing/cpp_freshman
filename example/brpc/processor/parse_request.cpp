#include "parse_request.h"


namespace utopian {
namespace ranker {

int ParseRequestProcessor::process() noexcept {
        //  *z.emit() = *x + *y;
        RequestInfo tmp;
        
        tmp.timeout = request->timeout();
        tmp.one_id = request->one_id();
        tmp.adx = request->adx();
        tmp.price = 1000;
        tmp.request_id = request->request_id();
        *normalized_request.emit() = std::move(tmp);
    }
   
}
}