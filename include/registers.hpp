#ifndef REGISTERS_HPP
#define REGISTERS_HPP

#include <cstdint>

uint16_t pc;

// true -> Fetch opcode
// false -> Fetch data
bool sync;

// Accumulator
uint8_t A;

// Carry
uint8_t C = 0;

#endif
