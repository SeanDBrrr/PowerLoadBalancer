#ifndef _IStation_HPP
#define _IStation_HPP

enum StationModes {
    Dynamic=1,
    FCFS,
    Director,
};

class IStation
{
public:
    virtual ~IStation(){}
    virtual int getId() = 0;
    virtual void charge(int power) = 0;
    virtual void switchMode(StationModes mode) = 0;
};

#endif