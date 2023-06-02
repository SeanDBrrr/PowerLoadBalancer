#ifndef IPLB_H
#define IPLB_H
#include <Arduino.h>
#include <string.h>

enum class BuildingEvents {NoEvent,
                          EV_SendSolarPower,
                          EV_ChargeBuilding};

class IPLB
{
    public:
    virtual void supplyPowerToBuilding(double power) = 0;
    virtual double getPower() = 0;
};

#endif