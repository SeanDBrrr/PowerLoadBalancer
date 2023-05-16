#ifndef PLBENUMS_HPP
#define PLBENUMS_HPP

enum PLBEvents {NoEvent = 0, 
                EV_timeout, 
                EV_supply1, 
                EV_supply2, 
                EV_supply3, 
                EV_supply4,
                EV_stop, 
                EV_director1,
                EV_director2,
                EV_director3,
                EV_director4};

enum PLBStates {ST_Idle=1, 
                ST_NoDir, 
                ST_Dir1, 
                ST_Dir2, 
                ST_Dir3, 
                ST_Dir3Only};

enum PLBModes {MO_Auto=1, 
               MO_Manual};


#endif