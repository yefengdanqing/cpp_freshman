#pragma once 
#include "dict_base.h"

namespace utopian {
namespace ranker {
class FileDict : public DictBase {
public:
    bool init() override {
        return true;
    }
    bool load() override {
        return true;
    }

    const std::string get_cur_file() {
        return _dict_file;
    }
    virtual ~FileDict() {

    }
protected:
    std::string _dict_file;


};


}
}