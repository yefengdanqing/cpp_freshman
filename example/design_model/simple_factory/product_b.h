#ifndef SIMPLE_FACTORY_PRODUCT_B_H__
#define SIMPLE_FACTORY_PRODUCT_B_H__

#include "product.h"

class ProductB : public Product
{
public:
    virtual void who() const override;
    virtual ~ProductB();
};



#endif