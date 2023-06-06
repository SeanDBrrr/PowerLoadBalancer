#ifndef _IBuilding_HPP
#define _IBuilding_HPP

#include <Arduino.h>

class IBuilding
{
public:
    virtual ~IBuilding() = default;
    virtual float calculateSolarPower() = 0;
    virtual void charge(float power) = 0;
    virtual float getCurrentSolarPower() = 0;
    virtual void notifyDashboard(String message) = 0;
};


#endif