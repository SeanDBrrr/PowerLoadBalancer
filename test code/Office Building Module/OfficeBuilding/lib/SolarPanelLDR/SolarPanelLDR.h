#ifndef SOLARPANEL_H
#define SOLARPANEL_H

#include <Arduino.h>
//#include "LDR.h"
#include "ISolarPanel.h"

class SolarPanelLDR : public ISolarPanel
{
private:
    int _solarPower;
    const int _ldrPin;
    const double minInputADC = 0;
    const double maxInputADC  = 4095;
    const double minOutputSolar = 0;
     const double maxOutputSolar = 2.5;
     double map(double power, double in_min, double in_max, double out_min, double out_max);

public:
    SolarPanelLDR(int pin);
    ~SolarPanelLDR() {}
    double getSolarPower();
};

#endif