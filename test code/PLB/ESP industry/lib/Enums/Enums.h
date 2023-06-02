#pragma once

enum class PLBEvents {
    noEvent, 
    EV_Timeout, 
    EV_Supply,
    EV_Stop, 
    EV_Director
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