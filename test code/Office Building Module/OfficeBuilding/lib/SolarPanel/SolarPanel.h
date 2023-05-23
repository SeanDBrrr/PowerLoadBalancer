#ifndef SOLARPANEL_H
#define SOLARPANEL_H
#include <Arduino.h>
#include "ISolarPanel.h"
#include "LDR.h"

class SolarPanel : public ISolarPanel
{
private:
    int _solarPower;
    LDR _ldr;

public:
    SolarPanel(int pin);
    ~SolarPanel() {}
    int getSolarPower();
};

#endif