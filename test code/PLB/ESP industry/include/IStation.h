#ifndef _IStation_HPP
#define _IStation_HPP

enum StationModes {
    MO_Dynamic=1,
    MO_FCFS,
    MO_Director,
};

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

class IStation
{
public:
    virtual ~IStation() = default;
    virtual int getId() = 0;
    virtual void charge(float power) = 0;
    virtual void switchMode(StationModes mode) = 0;
};

#endif