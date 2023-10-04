#include "memory.hpp"

void Memory::tick()
{
    uint16_t address = addressBus.read();
    uint8_t data = get(address);
    dataBus.write(data);
}

void DataBus::write(uint8_t data)
{
    _dataBuffer = data;
}

uint8_t DataBus::read()
{
    return _dataBuffer;
}

void DataBus::tock()
{
    _dataBus = _dataBuffer;
}
