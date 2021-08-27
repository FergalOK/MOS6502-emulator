#include <stdint.h>
#include <iostream>

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

    // Bi-directional
    uint8_t dataBus = 0; // pins 21-28 (D0-D7)
};

class externalController
{
    cpu6502Interface interface;
    bool phaseOne = false;
    bool phaseTwo = false;
    uint8_t memory[2048];
    uint8_t data;

    void cycle()
    {
        // Wait rise time (max 25ns)
        phaseOne = true;
        // Wait max(R/W setup, addr setup) (typically 200ns)
        bool reading = interface.reading; // R/W from CPU
        uint16_t addr = interface.addressBus; // address from CPU

        if (reading)
        {
            data = memory[addr];
            // Wait for Memory to read address and set data (max 500ns)
            // Pulse changes midway through
            phaseOne = false;
            phaseTwo = true;
            interface.dataBus = data;
            // Wait for CPU to read data (min 100ns)
            phaseTwo = false;
            // Data should be kept for min 10ns after the end of the cycle
        }
        else
        {
            // Wait for phase 2
            data = interface.dataBus; 
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