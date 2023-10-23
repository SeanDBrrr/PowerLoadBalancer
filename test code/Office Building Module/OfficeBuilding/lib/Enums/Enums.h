#pragma once
enum class Event
{
    NoEvent,
    EV_KEY_OPEN,
    EV_KEY_CLOSE,
    EV_SEND_SOLAR_POWER,
    EV_CHARGE_BUILDING,
    EV_WIFI_TRIALS,
    EV_MQTT_TRIALS,
    EV_WIFI_NOT_CONNECTED,
    EV_WIFI_CONNECTED,
    EV_MQTT_NOT_CONNECTED,
    EV_MQTT_CONNECTED,
    EV_SWITCH_STATE
};

enum class State
{
    STATE_OPEN,
    STATE_CLOSED
};