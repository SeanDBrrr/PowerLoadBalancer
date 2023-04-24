#include "OfficeBuilding.h"

void OfficeBuilding::HandleEvent(Events ev)
{
    switch (currentState)
    {
    case STATE_ACTIVE:
        currentState = HandleActive(ev);
        break;
    case STATE_INACTIVE:
        currentState = HandleInActive(ev);
        break;
    default:
        break;
    }
}

States OfficeBuilding::HandleActive(Events ev)
{
    States result = STATE_ACTIVE;
    display.showPowerOnDisplay(currentPower);

    switch (ev)
    {
    case EV_CLOSE:
        statebuilding.openClosed(officeState);
        result = STATE_INACTIVE;
        break;
    case EV_TIME_OUT:
        totalSolarPower = solarpanel.getSolarPower();
        //plb.Send(client, totalSolarPower);
        currentPower = plb.supplyPower();
        result = STATE_ACTIVE;
        break;
    default:
        break;
    }

    return result;
}

States OfficeBuilding::HandleInActive(Events ev)
{
    States result = STATE_INACTIVE;
    display.showPowerOnDisplay(currentPower);

    switch (ev)
    {
    case EV_OPEN:
        statebuilding.openClosed(officeState);
        result = STATE_ACTIVE;
        break;
    default:
        break;
    }

    return result;
}