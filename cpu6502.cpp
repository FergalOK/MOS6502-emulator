#include <stdint.h>
#include <iostream>
#include <stdio.h>

class cpu6502Interface
{
    // TODO: pins are wrong
    public:
    // Input
    void tick(); // pin 29 (CLK)
    void reset(); // pin 03 (¬RST)
    void interrupt(); // pin 32 (¬IRQ)
    void nminterrupt(); // pin 33 (¬NMI)

    // Output
    uint16_t addressBus = 0; // pins 04-19 (A0-A15)
    bool reading = true; // pin 34 (R/W) Read when high, write when low
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
    uint8_t data = 0;
    uint8_t lenData = 0;

    uint8_t accum = 0;
    uint8_t add = 0; // Adder hold register
    uint8_t carry = 0;

    uint8_t instrLen(uint8_t instr)
    {
        // TODO
        return 3;
    }

    void fetchData(uint16_t addr)
    {
        interface->addressBus = addr;
        interface->reading = true;
        interface->sync = false;
    }

    void fetchOpcode()
    {
        interface->addressBus = pc;
        interface->reading = true;
        interface->sync = true;
        timing = -1;
    }

    void store(uint16_t a, uint8_t d)
    {
        interface->addressBus = a;
        interface->dataBus = d;
        interface->reading = false;
    }

    void execute()
    {
        if (timing == 0)
        {
            // Decoding the instruction comes after fetching the current cycle's next data byte
            // Consequently, all instructions take at least 2 cycles
            fetchData(pc);
            pc++;
        }
        // Execute instruction
        else if (instr == 0xA9)
        {
            // LDA #Oper
            // Immediate Addressing

            if (timing == 1)
            {
                accum = data;
                fetchOpcode();
                pc++;
            }
        }
        else if (instr == 0x85)
        {
            // STA Oper
            // Zero Page
            if (timing == 1)
            {
                store(data, accum);
                // Don't increment pc
            }
            else if (timing == 2)
            {
                fetchOpcode();
                pc++;
            }
        }
        else if (instr == 0xEA)
        {
            // NOP
            // Implied
            if (timing == 1)
            {
                // fetchOpcode();
                // repeating fetch current pc
                timing = -1;
                interface->sync = 1;
            }
        }
        else if (instr == 0x4C)
        {
            // JMP Oper
            // Absolute
            if (timing == 1)
            {
                // https://retrocomputing.stackexchange.com/questions/19750/where-does-the-6502-store-the-low-order-byte-after-fetching-for-read-instruction
                // ADL help in add out
                add = data;
                fetchData(pc);
                pc++;
            }
            else if (timing == 2)
            {
                pc = add | (data << 8);
                fetchOpcode();
                pc++;
            }
        }
        else if (instr == 0x90)
        {
            // BCC
            // Relative
            if (timing == 1)
            {
                fetchOpcode();
                pc++;
                if (!carry)
                {
                    timing = -2;
                    // pc is 1 ahead of
                    pc += data-1;
                }
            }
        }
    }

    public:
    internalController(cpu6502Interface* _interface): interface(_interface) {}
    void cycle()
    {
        if (interface->sync) printf("------\n");
        printf("%i, %i, %i\n", pc, interface->addressBus, interface->dataBus);
        timing++;
        if (interface->sync && timing != 0)
        {
            interface->addressBus = pc;
            interface->reading = true;
            pc++;
            return;
        }

        if (timing == 0)
        {
            instr = interface->dataBus;
            interface->addressBus = pc;
        }
        else
        {
            data = interface->dataBus;
        }
        execute();
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
    externalController(cpu6502Interface* _interface): interface(_interface) 
    {
        memory[0] = 0xEA;
        memory[1] = 0x90;
        memory[2] = 4;
        memory[3] = 0x4C;
        memory[4] = 0x23;
        memory[5] = 0x01;
        memory[6] = 0xA9;
        memory[7] = 10;
        memory[int(0x123)] = 0xA9;
        memory[int(0x124)] = 20;
        
    }

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
    controller.cycle();
    controller.cycle();
    controller.cycle();
    controller.cycle();
    controller.cycle();
    controller.cycle();
    controller.cycle();
    controller.cycle();
    controller.cycle();
    controller.cycle();
}