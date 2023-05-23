#pragma once

enum class Event
{
    EV_PLUGGED,
    EV_UNPLUGGED,
    EV_INVALID_RFID,
    EV_RFID_DIRECTOR_DETECTED,
    EV_START,
    EV_CHARGING,
    EV_DONE_CHARGING,
    EV_ERROR,
    EV_RESET,
    EV_STOP
};