#include "SolarPanel.h"

SolarPanel::SolarPanel(int pin) : _solarPower{0}, _ldr{pin} {}

int
SolarPanel::getSolarPower()
{
    //return _ldr.readPin();  
    int random = 10;
    return random;
}