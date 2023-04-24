#ifndef IDISPLAY_H
#define IDISPLAY_H

class IDisplay
{
private:
    /* data */
public:
    IDisplay(){}
    ~IDisplay(){}

    virtual void showPowerOnDisplay(int currentPower); 
};

#endif