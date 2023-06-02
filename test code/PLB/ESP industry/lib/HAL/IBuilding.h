#ifndef _IBuilding_HPP
#define _IBuilding_HPP

class IBuilding
{
public:
    virtual ~IBuilding() = default;
    virtual float calculateSolarPower() = 0;
    virtual void charge(float power) = 0;
    virtual float getCurrentSolarPower() = 0;
};


#endif