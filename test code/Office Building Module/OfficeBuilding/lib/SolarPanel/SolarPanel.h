#ifndef SOLARPANEL_H
#define SOLARPANEL_H

#include <Arduino.h>
#include "LDR.h"
#include "ISolarPanel.h"

class SolarPanel : public ISolarPanel
{
private:
    int _solarPower;
    const int _ldrPin;
    double map(double power,double in_min, double in_max, double out_min, double out_max);
public:
    SolarPanel(int pin);
    ~SolarPanel() {}
    double getSolarPower();
};

#endif