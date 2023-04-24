#ifndef ISTATEBUILDING_H
#define ISTATEBUILDING_H

class IStateBuilding
{  
    public:
    IStateBuilding(){}
    ~IStateBuilding(){}

    virtual void openClosed(bool state);
};

#endif