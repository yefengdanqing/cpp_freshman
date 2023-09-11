#pragma once

#include <string>
#include <iostream>

class AbstractPhoneProduct {

public:
    virtual void call() = 0;
    virtual void bell() = 0;
};