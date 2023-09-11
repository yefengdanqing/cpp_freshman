#pragma once

#include <string>
#include <iostream>

class AbstractPcProduct {

public:
    virtual void work() = 0;
    virtual void down() = 0;
};