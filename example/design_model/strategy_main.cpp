#include "strategy/context.h"

void client_code() {
  
    auto ptr = std::make_shared<Context>(std::make_unique<CarMapStrategy>());
    // auto ptr = new Context(std::make_unique<CarMapStrategy>());
    ptr->context_do_something();
    ptr->set_strategy(std::make_unique<BusMapStrategy>());
    ptr->context_do_something();
    ptr->set_strategy(std::make_unique<BikeMapStrategy>());
    ptr->context_do_something();
    // Context context(std::make_unique<CarMapStrategy>());
    // context.context_do_something();
    // context.set_strategy(std::make_unique<BusMapStrategy>());
    // context.context_do_something();

}

int main() {
    client_code();
    /*
    Library* base = new Application_xxx;
    library_code(base);
    */
    return 0;
}