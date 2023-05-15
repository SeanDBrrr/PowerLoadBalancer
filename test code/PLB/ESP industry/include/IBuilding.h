#ifndef _IBuilding_HPP
#define _IBuilding_HPP

class IBuilding
{
public:
    virtual ~IBuilding() = default;
    virtual int calculateSolarPower() = 0;
    virtual void charge(float power) = 0;
};


#endif