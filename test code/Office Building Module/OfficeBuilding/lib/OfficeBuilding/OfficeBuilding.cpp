#include "OfficeBuilding.h"

OfficeBuilding::OfficeBuilding(IPLB *plb,
                               IDisplay *display,
                               ISolarPanel *panel1,
                               ISolarPanel *panel2,
                               ISolarPanel *panel3,
                               ISolarPanel *panel4) : _plb(plb),
                                                      _display(display),
                                                      _wifiTrials(0),
                                                      _mqttTrials(0),
                                                      _previousErrTime(0),
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

void OfficeBuilding::handleEvent(BuildingEvents ev)
{

    switch (ev)
    {
    case BuildingEvents::EV_SendSolarPower:
        sendSolarPower(calculateSolarPower());
        Serial.println("EV_SendSolarPower");
        break;
    case BuildingEvents::EV_ChargeBuilding:
        _display->display(static_cast<String>(_plb->getPower()));
        break;
    case BuildingEvents::EV_WIFI_TRIALS:
        Serial.println("EV_WIFI_TRIALS");
        // if (_wifiTrials < TRIALS)
        // {
            _wifiTrials++;
            _display->display("Connecting WiFi:", "Attempts: " + static_cast<String>(_wifiTrials));
        // }
        break;
    case BuildingEvents::EV_MQTT_TRIALS:
        Serial.println("EV_MQTT_TRIALS");
        // if (_mqttTrials < TRIALS)
        // {
            _mqttTrials++;
            _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
        // }
        break;
    case BuildingEvents::EV_WIFI_NOT_CONNECTED:
        Serial.println("EV_WIFI_NOT_CONNECTED");
        _display->display("ERROR:", "WiFi Connection");
        break;
    case BuildingEvents::EV_MQTT_NOT_CONNECTED:
        Serial.println("EV_MQTT_NOT_CONNECTED");
        _display->display("ERROR:", "MQTT Connection");
        break;
    case BuildingEvents::EV_WIFI_CONNECTED:
        _wifiTrials = 0;
        _mqttTrials = 0;
        Serial.println("EV_WIFI_CONNECTED");
        _display->display("WIFI:", "Connected!");
        break;
    case BuildingEvents::EV_MQTT_CONNECTED:
        _mqttTrials = 0;
        Serial.println("EV_MQTT_CONNECTED");
        _display->display("MQTT:", "Connected");
        break;
    }
}
void OfficeBuilding::sendSolarPower(int solarPower)
{
    _plb->supplyPowerToBuilding(solarPower);
}

void OfficeBuilding::loop(BuildingEvents ev)
{
    handleEvent(ev);
}