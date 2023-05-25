#ifndef SOLARPANEL_H
#define SOLARPANEL_H

#include <Arduino.h>
#include "LDR.h"
#include "ISolarPanel.h"

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