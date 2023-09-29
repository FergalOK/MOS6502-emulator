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
    // The clock provides the cycle we are at in the pipelining
    uint8_t _cycle = 0;
    std::vector<ClockSubscriber*> _subscribers;

public:
    void subscribe(ClockSubscriber* subscriber);
    void run();
};

#endif
