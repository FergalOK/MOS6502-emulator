#include "microprocessor.hpp"
#include "registers.hpp"

#include <map>

extern AddressBus addressBus;
extern DataBus dataBus;

std::map<uint8_t, uint8_t> opCycles = {
    {0x6D, 4}
};

uint8_t ALU;

std::map<std::pair<uint8_t, uint8_t>, uint8_t*> dataLocation = {
    {{0x6D, 1}, &ALU}
};

void Microprocessor::tick(uint8_t cycle)
{
    if (sync) {
        _currentOp = dataBus.read();
        sync = false;
        addressBus.write(++pc);
        return;
    }

    if (_currentOp == 0x6D) {
        if (cycle == 1) {
            // Store in ADL in ALU
            ALU = dataBus.read();
            addressBus.write(++pc);
        }
        else if (cycle == 2) {
            uint8_t ADL = ALU;
            uint8_t ADH = dataBus.read();
            uint16_t address = (ADH << 8) | ADL;
            addressBus.write(address);
        }
        else if (cycle == 3) {
            // TODO: Is it actually stored in the ALU?
            ALU = dataBus.read();
        }
        else if (cycle == 4) {
            uint8_t result = ALU + A + C; 
        }
    }

    if (cycle >= opCycles[_currentOp]) {
        // All data available for op:
        // Fetch new instruction
        sync = true;
        addressBus.write(pc++);
        *dataLocation[{_currentOp, cycle}] = dataBus.read();
    }
}

void Microprocessor::tock(uint8_t cycle)
{
    dataBus.read();
}


/*
Cases:
- Need opcode: Put pc on address bus. Set sync true
- Need data: Put data address on address bus. Set sync false
- 
*/