#ifndef _IBuilding_HPP
#define _IBuilding_HPP

enum PLBEvents {noEvent = 0, 
                EV_timeout, 
                EV_supply1, 
                EV_supply2, 
                EV_supply3, 
                EV_supply4,
                EV_stop, 
                EV_director1,
                EV_director2,
                EV_director3,
                EV_director4};

class IBuilding
{
public:
    virtual ~IBuilding() = default;
    virtual int calculateSolarPower() = 0;
    virtual void charge(float power) = 0;
};


#endif