#include "OfficeBuilding.h"

OfficeBuilding::OfficeBuilding(IPLB *plb,
                               IDisplay *display,
                               IOfficeState *officeState,
                               ISolarPanel *panel1,
                               ISolarPanel *panel2,
                               ISolarPanel *panel3,
                               ISolarPanel *panel4) : _plb(plb),
                                                      _display(display),
                                                      _officeState(officeState),
                                                      _currentState(State::STATE_OPEN),
                                                      _wifiTrials(0),
                                                      _mqttTrials(0),
                                                      _previousTime(0),
                                                      _currentSolarPower(0)
{
    _solarPanels.emplace_back(panel1);
    _solarPanels.emplace_back(panel2);
    _solarPanels.emplace_back(panel3);
    _solarPanels.emplace_back(panel4);
}

OfficeBuilding::~OfficeBuilding()
{
    delete _display;
    for (auto &sp : _solarPanels)
    {
        delete sp;
    }
    delete _plb;
}

double OfficeBuilding::calculateSolarPower()
{
    _currentSolarPower = 0;
    for (const auto &sp : _solarPanels)
    {

        _currentSolarPower += sp->getSolarPower();
    }
    return _currentSolarPower;
}

void OfficeBuilding::addSolarPanel(ISolarPanel *panel)
{
    _solarPanels.emplace_back(panel);
}

void OfficeBuilding::handleEvent(Event ev)
{

    switch (ev)
    {
    case Event::EV_SEND_SOLAR_POWER:
        sendSolarPower(calculateSolarPower());
        break;
    case Event::EV_CHARGE_BUILDING:
        _display->display("Consumption:", static_cast<String>(_plb->getPower()) + "kW");
        break;
    case Event::EV_WIFI_TRIALS:
        _wifiTrials++;
        _display->display("Connecting WiFi:", "Attempts: " + static_cast<String>(_wifiTrials));
        break;
    case Event::EV_MQTT_TRIALS:
        _mqttTrials++;
        _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
        break;
    case Event::EV_WIFI_NOT_CONNECTED:
        _display->display("ERROR:", "WiFi Connection");
        break;
    case Event::EV_MQTT_NOT_CONNECTED:
        _display->display("ERROR:", "MQTT Connection");
        break;
    case Event::EV_WIFI_CONNECTED:
        _wifiTrials = 0;
        _mqttTrials = 0;
        _display->display("WIFI:", "Connected!");
        break;
    case Event::EV_MQTT_CONNECTED:
        _mqttTrials = 0;
        _display->display("MQTT:", "Connected");
        break;
    case Event::EV_SWITCH_STATE:
        _plb->sendStateToPLB(_currentState);
        break;
    }
}

void OfficeBuilding::sendSolarPower(double solarPower)
{
    _plb->supplyPowerToBuilding(solarPower);
}

void OfficeBuilding::loop(Event &ev)
{
    _events.emplace_back(std::move(ev));

    if (_currentState == State::STATE_OPEN)
    {
        if (_officeState->isOpen())
        {
            _display->display("Building state", "CLOSED");
            _currentState = State::STATE_CLOSED;
            _events.emplace_back(Event::EV_SWITCH_STATE);
        }
    }
    else if (_currentState == State::STATE_CLOSED)
    {
        if (_officeState->isOpen())
        {
            _display->display("Building state", "OPEN");
            _currentState = State::STATE_OPEN;
            _events.emplace_back(Event::EV_SWITCH_STATE);
        }
    }

    for (const auto &ev : _events)
        {
            handleEvent(ev);
        }
        
        _events.clear();
    }