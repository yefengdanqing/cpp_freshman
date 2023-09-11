#ifndef CPP_FRESHMAN_DESIGN_MODEL_STRATEGY_STRATEGY_H__
#define CPP_FRESHMAN_DESIGN_MODEL_STRATEGY_STRATEGR_H__

class MapStrategy
{
public:
    virtual void route() const = 0;
    virtual ~MapStrategy() = default;
    virtual void who() const = 0;
};

class CarMapStrategy : public MapStrategy {
public:
    virtual void route() const override;
    virtual void who() const override;
};

class BikeMapStrategy : public MapStrategy {
public:
    virtual void route() const override;
    virtual void who() const override;
};

class BusMapStrategy : public MapStrategy {
public:
    virtual void route() const override;
    virtual void who() const override;
};






#endif