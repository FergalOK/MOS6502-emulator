#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <vector>

class ClockSubscriber
{
public:
    virtual void tick() = 0;
};

class Clock
{
private:
    std::vector<ClockSubscriber> _subscribers;

public:
    void run();
};

#endif
