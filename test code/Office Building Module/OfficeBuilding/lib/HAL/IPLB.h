#ifndef IPLB_H
#define IPLB_H
#include <Arduino.h>
#include <string.h>

enum class BuildingEvents {NoEvent,
                          EV_SendSolarPower,
                          EV_ChargeBuilding,
                          EV_WIFI_TRIALS,
                          EV_MQTT_TRIALS,
                          EV_WIFI_NOT_CONNECTED,
                          EV_WIFI_CONNECTED,
                          EV_MQTT_NOT_CONNECTED,
                          EV_MQTT_CONNECTED};

class IPLB
{
    public:
    virtual void supplyPowerToBuilding(double power) = 0;
    virtual double getPower() = 0;
};

#endif