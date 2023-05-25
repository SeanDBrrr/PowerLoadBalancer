#include "OfficeBuilding.h"

OfficeBuilding::OfficeBuilding(IPLB *plb, 
                               IDisplay *display, 
                               ISolarPanel *panel1, 
                               ISolarPanel *panel2, 
                               ISolarPanel *panel3, 
                               ISolarPanel *panel4)
{
    _plb = plb;
    _display = display;
    _solarPanels.emplace_back(panel1);
    _solarPanels.emplace_back(panel2);
    _solarPanels.emplace_back(panel3);
    _solarPanels.emplace_back(panel4);
}

OfficeBuilding::~OfficeBuilding() 
{
    delete _display;
    for (auto &sp: _solarPanels)
    {
        delete sp;
    }
    delete _plb;    
}

int 
OfficeBuilding::calculateSolarPower()
{
    currentSolarPower = 0;
    for (const auto &sp: _solarPanels)
    {
        currentSolarPower += sp->getSolarPower();
    }
}

void 
OfficeBuilding::addSolarPanel(ISolarPanel *panel)
{
    _solarPanels.emplace_back(panel);
}

void OfficeBuilding::handleEvent(BuildingEvents ev)
{
    switch (ev)
    {
    case BuildingEvents::EV_RequestSolarPower: 
        calculateSolarPower();
        requestPower();
        break;
    default:
        break;
    }
}

void 
OfficeBuilding::requestPower()
{
    _plb->supplyPowerToBuilding();
}

void
OfficeBuilding::loop()
{
    // handleEvent();
}