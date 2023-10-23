#include "SolarPanelLDR.h"

SolarPanelLDR::SolarPanelLDR(int pin) : _solarPower{0}, _ldrPin{pin} {}

double SolarPanelLDR::map(double power, double in_min, double in_max, double out_min, double out_max)
{
  if (in_min >= in_max || out_min >= out_max) {
  }
  
  if (power < in_min) {
    power = in_min;
  } else if (power > in_max) {
    power = in_max;
  }
  
  return (power - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double SolarPanelLDR::getSolarPower()
{
    int read, solarPower;
    read = analogRead(_ldrPin);
    solarPower = map(read,minInputADC,maxInputADC,minOutputSolar,maxOutputSolar);
    Serial.print("SolarPower: ");
    Serial.println(solarPower);
    return solarPower;
}