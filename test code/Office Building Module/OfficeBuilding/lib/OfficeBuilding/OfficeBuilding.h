#ifndef OFFICEBUILDING_H
#define OFFICEBUILDING_H
#include <vector>
#include <iostream>
#include <Arduino.h>
#include "IOfficeState.h"
#include "IDisplay.h"
#include "IPLB.h"
#include "ISolarPanel.h"
#include "IPLB.h"
#include "Enums.h"

class OfficeBuilding
{
private:
    IPLB *_plb;
    IDisplay *_display;
    IOfficeState *_officeState;
    State _currentState;
    std::vector<ISolarPanel *> _solarPanels;
     std::vector<Event> _events;
    int _wifiTrials;
    int _mqttTrials;
    unsigned long _previousTime;
    const int _INTERVAL = 1000;
    const int _TRIALS = 40;
    double _currentSolarPower;
    bool _officeFlagState = false;
    bool _lastOfficeState = false;
    bool _readOfficeState = false;

public:
    OfficeBuilding(IPLB *plb,
                   IDisplay *display,
                   IOfficeState *officeState,
                   ISolarPanel *panel1,
                   ISolarPanel *panel2,
                   ISolarPanel *panel3,
                   ISolarPanel *panel4);
    ~OfficeBuilding();

    void addSolarPanel(ISolarPanel *panel);
    void handleEvent(Event ev);
    double calculateSolarPower();
    void sendSolarPower(double power);
    void loop(Event &ev);
};

#endif