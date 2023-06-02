#pragma once
#include <stdint.h>
#include <Enums.h>

class IPLB
{
public:
    virtual ~IPLB() = default;
    virtual float getPowerReceived() = 0;
    virtual void supplyPowerToStation(int id) = 0;
    virtual void stopSupplyToStation(int id) = 0;
    virtual void checkDirector(uint32_t id) = 0;
    //virtual DirectorState directorTimeout (int waitingTime) = 0;
};