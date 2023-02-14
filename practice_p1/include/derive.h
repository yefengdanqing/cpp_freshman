#include "base.h"

#include<string>

namespace practice_p1 {
class Derive : public Base {
private:
    int index;
    std::string name;
    float m_sum_a = 0;
    float m_sum_b = 0;
    double price = 0;
public:
    virtual void get_name();
    virtual float add_sum();
    virtual int get_index();
    void print1();

};
}