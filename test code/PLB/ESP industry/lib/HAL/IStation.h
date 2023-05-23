#ifndef _IStation_HPP
#define _IStation_HPP

enum StationModes {
    MO_Dynamic=1,
    MO_FCFS,
    MO_Director,
};

enum PLBEvents {noEvent = 0, 
                EV_Timeout, 
                EV_Supply1, 
                EV_Supply2, 
                EV_Supply3, 
                EV_Supply4,
                EV_Stop1,
                EV_Stop2,
                EV_Stop3,
                EV_Stop4, 
                EV_Director1,
                EV_Director2,
                EV_Director3,
                EV_Director4};

class IStation
{
public:
    virtual ~IStation() = default;
    virtual int getId() = 0;
    virtual int getDirectorId() = 0;
    virtual void charge(float power) = 0;
    virtual void switchMode(StationModes mode) = 0;
};

#endif