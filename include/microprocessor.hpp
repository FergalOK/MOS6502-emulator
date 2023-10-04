#ifndef MICROPROCESSOR_HPP
#define MICROPROCESSOR_HPP

#include "memory.hpp"
#include "clock.hpp"

class Microprocessor: public ClockSubscriber
{
private:
    uint8_t _currentOp;
public:
    void tick(uint8_t cycle) override;
    void tock(uint8_t cycle) override;
};

#endif
