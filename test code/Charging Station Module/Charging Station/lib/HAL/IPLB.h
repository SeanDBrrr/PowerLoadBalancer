#pragma once
#include <stdint.h>

class IPLB
{
public:
    virtual ~IPLB() = default;
    virtual void supplyPowerToStation(int id) = 0;
    virtual void stopSupplyToStation(int id) = 0;
    virtual void checkDirector(uint32_t id) = 0;
};