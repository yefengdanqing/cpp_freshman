#pragma once
#include <any>
#include <string>

namespace utopian {
namespace ranker {
class DictBase {
public:
    DictBase(){}
    virtual ~DictBase() {

    }
    virtual bool init() = 0;
    virtual bool load() = 0;

    virtual const std::string get_cur_file() = 0;



};

}
}
