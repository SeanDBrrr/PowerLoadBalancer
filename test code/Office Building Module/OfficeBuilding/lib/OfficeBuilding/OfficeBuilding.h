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
    int _wifiTrials;
    int _mqttTrials;
    unsigned long _previousTime;
    const int _INTERVAL = 1000;
    const int _TRIALS = 40;
    double _currentSolarPower;
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
    double calculateSolarPower();
    void sendSolarPower(double power);
    void loop(BuildingEvents ev);
};

#endif