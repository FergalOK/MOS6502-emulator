#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <vector>
#include <cstdint>

class ClockSubscriber
{
public:
    virtual void tick(uint8_t cycle) = 0;
};

class Clock
{
private:
    std::vector<ClockSubscriber> _subscribers;

public:
    void run();
};

#endif
