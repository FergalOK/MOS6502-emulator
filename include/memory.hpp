#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>

#define MEM_SIZE 1 << 16

// Programmer sets ROM, RAM, IO ranges
uint8_t MEM[MEM_SIZE];

class Bus
{
private:
    uint8_t _data;
public:
    uint8_t read();
    void write(uint8_t data);
};

Bus addressBus;
Bus dataBus; 

#endif
