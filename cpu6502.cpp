#include <stdint.h>
#include <iostream>
#include <stdio.h>

class cpu6502Interface
{
    public:
    // Input
    void tick(); // pin 29 (CLK)
    void reset(); // pin 03 (¬RST)
    void interrupt(); // pin 32 (¬IRQ)
    void nminterrupt(); // pin 33 (¬NMI)

    // Output
    uint16_t addressBus = 0; // pins 04-19 (A0-A15)
    bool reading = false; // pin 34 (R/W) Read when high, write when low
    uint8_t out = 0; // pins 37-39 (OUT2-OUT0) Only first 3 bits are used
    bool sync = true;

    // Bi-directional
    uint8_t dataBus = 0; // pins 21-28 (D0-D7)
};

class internalController
{
    cpu6502Interface* interface;
    // Hardware Manual:
    // "Each instruction starts in T0 and goes to T1, T2, T3, etc. 
    // for as many cycles as are required to complete execution of the instruction."
    uint8_t timing = -2;
    uint16_t pc = 0;
    uint8_t instr = 0;
    uint8_t lenData = 0;

    uint8_t instrLen(uint8_t instr)
    {
        // TODO
        return 3;
    }

    void execute(uint8_t instr)
    {
        // Execute instruction
    }

    public:
    internalController(cpu6502Interface* _interface): interface(_interface) {}
    void cycle()
    {
        timing++;
        if (interface->sync)
        {
            interface->addressBus = pc;
            interface->reading = true;
            pc++;
        }
        if (timing == -1) return;

        if (timing == 0)
        {
            instr = interface->dataBus;
            uint8_t len = instrLen(instr);
            execute(instr);
            // If instruction only requires the instruction byte,
            // the following fetched byte is discarded while the 1-byte
            // instruction is executing 
            if (len != 1) {
                pc++;
            }
        }
    }
};

class externalController
{
    cpu6502Interface* interface;
    bool phaseOne = false;
    bool phaseTwo = false;
    uint8_t memory[2048];
    uint8_t data;

    public:
    externalController(cpu6502Interface* _interface): interface(_interface) {}

    void cycle()
    {
        // Wait rise time (max 25ns)
        phaseOne = true;
        // Wait max(R/W setup, addr setup) (typically 200ns)
        bool reading = interface->reading; // R/W from CPU
        uint16_t addr = interface->addressBus; // address from CPU

        if (reading)
        {
            data = memory[addr];
            // Wait for Memory to read address and set data (max 500ns)
            // Pulse changes midway through
            phaseOne = false;
            phaseTwo = true;
            interface->dataBus = data;
            // Wait for CPU to read data (min 100ns)
            phaseTwo = false;
            // Data should be kept for min 10ns after the end of the cycle
        }
        else
        {
            // Wait for phase 2
            data = interface->dataBus; 
            // data setup time
            phaseOne = false;
            phaseTwo = true;
            memory[addr] = data;
            // Wait for write to Memory
            phaseTwo = false;
            // Data should be kept for min 10ns after the end of the cycle
        }
    }
};

class controller
{
    cpu6502Interface* interface = new cpu6502Interface();
    internalController* internal = new internalController(interface);
    externalController* external = new externalController(interface);

    public:
    void cycle()
    {
        // TODO: Use 2 threads?
        internal->cycle();
        external->cycle();
    }
};

int main()
{
    controller controller;
    controller.cycle();
    controller.cycle();
}