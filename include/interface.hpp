#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "clock.hpp"

class PeripheralInterface: public ClockSubscriber
{
public:
    void tick(uint8_t cycle) override;
    void tock(uint8_t cycle) override;
};

#endif
