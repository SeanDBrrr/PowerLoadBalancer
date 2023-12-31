#pragma once

enum class Event
{
    noEvent = 0,
    EV_PLUGGED,
    EV_UNPLUGGED,
    EV_RFID_VALID,
    EV_RFID_INVALID,
    EV_RFID_TIMED_OUT,
    EV_RFID_ALREADY_CHECKED_IN,
    EV_RFID_DIRECTOR_DETECTED,
    EV_START,
    EV_CHARGING,
    EV_DONE_CHARGING,
    EV_ERROR,
    EV_RESET,
    EV_STOP,
    EV_MODE_CHANGED_DYNAMIC,
    EV_MODE_CHANGED_FCFS,
    EV_MODE_CHANGED_DIRECTOR,
    EV_WIFI_TRIALS,
    EV_MQTT_TRIALS,
    EV_WIFI_NOT_CONNECTED,
    EV_WIFI_CONNECTED,
    EV_MQTT_NOT_CONNECTED,
    EV_MQTT_CONNECTED,
    EV_PLB_CONNECTED,
    EV_PLB_DISCONNECTED
};

enum class State
{
    MAIN_STATE_WORKING,
    MAIN_STATE_ERROR,
    STATE_IDLE,
    STATE_VERIFYING_DIRECTOR,
    STATE_IDLE_DIRECTOR,
    STATE_PLUGGED,
    STATE_PLUGGED_DIRECTOR,
    STATE_WAITING_FOR_POWER,
    STATE_CHARGING,
    STATE_STOPPED_CHARGING,
    STATE_ERROR // maybe not needed? Needs to exit substate machine via this I think
};

enum class DirectorState
{
    noState,
    VALID,
    INVALID,
    ALREADY_CHECKED_IN,
    TIMED_OUT
};

enum class StationMode
{
    Dynamic = 1,
    FCFS,
    Director
};