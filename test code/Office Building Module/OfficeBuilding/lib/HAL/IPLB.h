#ifndef IPLB_H
#define IPLB_H
#include <Arduino.h>
#include <string.h>
#include "Enums.h"

class IPLB
{
    public:
    virtual void supplyPowerToBuilding(double power) = 0;
    virtual double getPower() = 0;
    virtual void sendStateToPLB(State buildingState) = 0;
};

#endif