#ifndef ISOLARPANEL_H
#define ISOLARPANEL_H

class ISolarPanel
{  
    public:
    ISolarPanel(){}
    ~ISolarPanel(){}

    virtual int getSolarPower() = 0;
};

#endif