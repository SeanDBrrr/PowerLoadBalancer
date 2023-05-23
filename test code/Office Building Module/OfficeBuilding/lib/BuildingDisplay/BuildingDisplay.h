#ifndef BUILDINGDISPLAY_H
#define BUILDINGDISPLAY_H

#include "IDisplay.h"

class BuildingDisplay : public IDisplay
{
private:
    int _scl;
    int _sda;
    LiquidCrystal_I2C _lcd;

public:
    BuildingDisplay(int scl, int sda);
    ~BuildingDisplay() {}
    void display(String);
};

#endif