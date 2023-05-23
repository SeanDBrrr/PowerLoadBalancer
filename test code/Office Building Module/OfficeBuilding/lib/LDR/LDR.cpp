#include "LDR.h"
#include <Arduino.h>

LDR::LDR(int pin) : _pin{pin} 
{}

int 
LDR::readPin()
{
    return analogRead(_pin);
}