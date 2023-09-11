#ifndef CPP_FRESHMAN_DESIGN_MODEL_TEMPLATE_METHOD_LIBRARY_H__
#define CPP_FRESHMAN_DESIGN_MODEL_TEMPLATE_METHOD_LIBRARY_H__
class Library {
public:
    virtual void run() final;
    void clock();
    void get_off_work();
    virtual void do_bussiness();
    
};

#endif