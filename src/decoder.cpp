#include "decoder.hpp"
#include "memory.hpp"

extern Bus dataBus;

void InstructionDecoder::tick(uint8_t cycle)
{
    if (cycle == 1) {
        uint8_t opcode = dataBus.read();
        parse(opcode);
    }
}
