#pragma once
class Command
{
private:
    /* data */
public:
    Command(/* args */) {}
    virtual ~Command() {}
    virtual bool execute() = 0;
};


