#ifndef ISOLARPANEL_H
#define ISOLARPANEL_H

class ISolarPanel
{  
    public:
    virtual ~ISolarPanel() = default;
    virtual int getSolarPower() = 0;
};

#endif