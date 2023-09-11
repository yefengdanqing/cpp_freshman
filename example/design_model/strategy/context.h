#ifndef CPP_FRESHMAN_DESIGN_MODEL_STRATEGY_CONTEXT_H__
#define CPP_FRESHMAN_DESIGN_MODEL_STRATEGY_CONTEXT_H__


#include <memory>
#include <iostream>


#include "map_strategy.h"


class Context {
public:
    explicit Context(std::unique_ptr<MapStrategy>&& str) : strategy(std::move(str)) {
        // strategy->who();
        //参数和成员变量一定不要一样命名
    }
    void context_do_something() {
        strategy->who();
        if (strategy == nullptr) {
            std::cout << "strategy == nullptr" << std::endl;
        } else {
            strategy->route();
            std::cout << "strategy finish" << std::endl;
        }
    }
    void set_strategy(std::unique_ptr<MapStrategy>&& str) {
        strategy = std::move(str);
        // strategy->who();

    }
private:
    std::unique_ptr<MapStrategy> strategy;
};


#endif