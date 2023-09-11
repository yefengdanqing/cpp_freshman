#pragma once

#include <string>
#include <iostream>

class AbstractTvProduct {

public:
    virtual void open() = 0;
    virtual void play() = 0;
};