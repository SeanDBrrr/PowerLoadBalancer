#pragma once

enum class PLBEvents {
    noEvent = 0, 
    EV_Timeout, 
    EV_Supply1, 
    EV_Supply2, 
    EV_Supply3, 
    EV_Supply4,
    EV_Stop1,
    EV_Stop2,
    EV_Stop3,
    EV_Stop4, 
    EV_Director1,
    EV_Director2,
    EV_Director3,
    EV_Director4
};

enum class PLBStates {
    ST_Idle = 1, 
    ST_NoDir, 
    ST_Dir1, 
    ST_Dir2, 
    ST_Dir3, 
    ST_Dir3Only,
    ST_Dir4
};

enum class PLBModes {
    MO_Auto = 1, 
    MO_Manual
};