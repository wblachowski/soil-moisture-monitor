#include <EEPROM.h>

class Memory
{
public:
    uint32_t readLastWatering();
    void saveLastWatering(uint32_t timestamp);
};