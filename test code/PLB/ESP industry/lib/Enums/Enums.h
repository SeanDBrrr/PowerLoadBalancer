#pragma once

enum class PLBEvents {
    noEvent, 
    EV_Timeout, 
    EV_Supply,
    EV_Stop, 
    EV_Director,
    EV_Disconnected,
    EV_Connected,
    EV_SwitchMode,
    EV_SwitchStationMode,
};

enum class PLBStates {
    ST_Idle, 
    ST_NoDir, 
    ST_Dir1, 
    ST_Dir2, 
    ST_Dir3, 
    ST_Dir3Only,
    ST_Dir4
};

enum class PLBModes {
    MO_Auto, 
    MO_Manual
};

enum class StationModes {
    MO_Dynamic,
    MO_FCFS,
    MO_Director,
};

enum class DirectorState {
    VALID ,
    INVALID,
    ALREADY_CHECKED_IN,
};