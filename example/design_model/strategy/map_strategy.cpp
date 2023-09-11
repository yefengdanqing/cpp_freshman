#include <iostream>
#include "map_strategy.h"

void CarMapStrategy::route() const {
    std::cout << "i am audi,and i fast" << std::endl;
}

void CarMapStrategy::who() const {
    std::cout << "i am audi" << std::endl;
}

void BikeMapStrategy::route() const  {
    std::cout << "i am bike, and it is free" << std::endl;
}
void BikeMapStrategy::who() const  {
    std::cout << "i am bike" << std::endl;
}

void BusMapStrategy::route() const {
    std::cout << "i am bus, and it is big" << std::endl;
}
void BusMapStrategy::who() const  {
    std::cout << "i am bus" << std::endl;
}