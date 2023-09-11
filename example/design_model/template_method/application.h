#ifndef CPP_FRESHMAN_DESIGN_MODEL_TEMPLATE_METHOD_APPLICATION_H__
#define CPP_FRESHMAN_DESIGN_MODEL_TEMPLATE_METHOD_APPLICATION_H__


#include "library.h"

class Application : public Library {
public:
    virtual void do_bussiness() override;

};


#endif