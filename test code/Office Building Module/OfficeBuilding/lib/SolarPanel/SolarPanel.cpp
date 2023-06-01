#include "SolarPanel.h"

SolarPanel::SolarPanel(int pin) : _solarPower{0}, _ldrPin{pin} {}

double SolarPanel::map(double power, double in_min, double in_max, double out_min, double out_max)
{
  if (in_min >= in_max || out_min >= out_max) {
    // Invalid input range, handle error or return default value
    // Example: throw an exception, return a default value, etc.
  }
  
  // Handle out-of-range input value
  if (power < in_min) {
    power = in_min;
  } else if (power > in_max) {
    power = in_max;
  }
  
  return (power - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double SolarPanel::getSolarPower()
{
    int read, solarPower;
    read = analogRead(_ldrPin);
    solarPower = map(read,0,4095,0,2.5);
    Serial.print("SolarPower: ");
    Serial.println(solarPower);
    return solarPower;
}