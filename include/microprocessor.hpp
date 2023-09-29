#ifndef MICROPROCESSOR_HPP
#define MICROPROCESSOR_HPP

#include "memory.hpp"
#include "clock.hpp"

class Microprocessor: public ClockSubscriber
{
public:
    void tick(uint8_t cycle) override;
};

#endif
