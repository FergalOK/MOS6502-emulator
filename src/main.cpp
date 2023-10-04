#include "microprocessor.hpp"
#include "interface.hpp"
#include "memory.h"
#include "clock.hpp"

#include <cstdint>

extern uint8_t MEM[];

int main()
{
    PeripheralInterface interface;
    Microprocessor microprocessor;
    Clock clock;

    clock.subscribe(&interface);
    clock.subscribe(&microprocessor);
    clock.run();

    return 0;
}
