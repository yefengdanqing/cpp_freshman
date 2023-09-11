#ifndef SIMPLE_FACTORY_PRODUCT_H__
#define SIMPLE_FACTORY_PRODUCT_H__

class Product
{
public:
    virtual ~Product();
    virtual void who() const = 0;
};







#endif