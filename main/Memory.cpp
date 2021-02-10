#include "Memory.h"
#include <EEPROM.h>
#define byte uint8_t

union ArrayToInteger
{
    byte array[4];
    uint32_t integer;
};

uint32_t Memory::readLastWatering()
{
    ArrayToInteger converter;
    for (int i = 0; i < 4; i++)
    {
        converter.array[i] = EEPROM.read(i);
    }
    return converter.integer;
}

void Memory::saveLastWatering(uint32_t timestamp)
{
    ArrayToInteger converter;
    converter.integer = timestamp;
    for (int i = 0; i < 4; i++)
    {
        EEPROM.write(i, converter.array[i]);
    }
}
