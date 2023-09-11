#ifndef SIMPLE_FACTORY_PRODUCT_A_H__
#define SIMPLE_FACTORY_PRODUCT_A_H__

#include "product.h"

class ProductA : public Product
{
public:
    void who() const override;
    virtual ~ProductA();
};

#endif