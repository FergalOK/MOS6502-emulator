#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>

#define MEM_SIZE 1 << 16

// Programmer sets ROM, RAM, IO ranges

class Memory
{
private:
    uint8_t MEM[MEM_SIZE];
public:
    uint8_t get(uint16_t address);
    void set(uint16_t address, uint8_t value);

    void tick();
};

class AddressBus
{
private:
    uint16_t _address;
public:
    uint8_t read();
    void write(uint16_t address);
};

class DataBus
{
private:
    uint8_t _dataBuffer;
    uint8_t _dataBus;
public:
    uint8_t read();
    void write(uint8_t data);

    void tock();
};

AddressBus addressBus;
DataBus dataBus;

// TODO: The ALU is more complicated than this
uint8_t ALU;

#endif
