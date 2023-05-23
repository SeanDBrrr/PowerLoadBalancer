#include "SolarPanel.h"

SolarPanel::SolarPanel(int pin)
{
    _solarPower = 0;
    _ldr = LDR(pin);
}

int
SolarPanel::getSolarPower()
{
    return _ldr.readPin();  
}