#ifndef _IBuilding_HPP
#define _IBuilding_HPP

class IBuilding
{
public:
    virtual ~IBuilding(){}
    virtual int getPowerProduced() = 0;
    virtual void charge(int power) = 0;
    virtual bool getState() = 0;
};


#endif