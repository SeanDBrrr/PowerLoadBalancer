#ifndef OFFICEBUILDING_H
#define OFFICEBUILDING_H
#include <vector>
#include <iostream>
#include <Arduino.h>

#include "IDisplay.h"
#include "IPLB.h"
#include "ISolarPanel.h"
#include "IPLB.h"

class OfficeBuilding
{
private:
    IPLB *_plb;
    IDisplay *_display;
    std::vector<ISolarPanel *> _solarPanels;

    int currentSolarPower;

public:
    OfficeBuilding(IPLB *plb, 
                   IDisplay *display, 
                   ISolarPanel *panel1, 
                   ISolarPanel *panel2, 
                   ISolarPanel *panel3, 
                   ISolarPanel *panel4);
    ~OfficeBuilding();
    
    void addSolarPanel(ISolarPanel *panel);
    void handleEvent(BuildingEvents ev); 
    int calculateSolarPower();
    void requestPower();
    void loop();

};

#endif