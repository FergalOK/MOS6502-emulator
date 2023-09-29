#ifndef DECODER_HPP
#define DECODER_HPP

#include "clock.hpp"

class InstructionDecoder : public ClockSubscriber
{
private:
    void parse(uint8_t opcode);
public:
    void tick(uint8_t cycle) override;
};

#endif
