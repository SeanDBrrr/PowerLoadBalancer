#pragma once

enum States
{
    MAIN_STATE_WORKING,
    MAIN_STATE_ERROR,
    STATE_IDLE,
    STATE_IDLE_DIRECTOR,
    STATE_PLUGGED,
    STATE_PLUGGED_DIRECTOR,
    STATE_WAITING_FOR_POWER,
    STATE_CHARGING,
    STATE_DONE_PLUGGED,
    STATE_ERROR//maybe not needed? Needs to exit substate machine via this I think
};