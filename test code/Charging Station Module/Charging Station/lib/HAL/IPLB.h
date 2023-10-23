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
    virtual void directorTimeout(int waitingTime) = 0;
    virtual void SetPowerRecievedFlag(bool powerRecievedFlag) = 0;
    virtual bool getPowerReceievedFlag() = 0;
    virtual void SetExplicitStopFlag(bool explicitStopFlag) = 0;
    virtual bool getExplicitStopFlag() = 0;
    virtual void callClientLoop();
};