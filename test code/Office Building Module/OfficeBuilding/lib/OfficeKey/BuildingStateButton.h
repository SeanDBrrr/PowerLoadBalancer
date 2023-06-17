#include "IOfficeState.h"
#include "Button.h"
#pragma once
const int BUTTON_PIN = 13; // TODO NEEDS TO BE CHANGED

class BuildingStateButton : public IOfficeState
{
private:
    SinglePressButton *_button;

public:
    BuildingStateButton();
    ~BuildingStateButton();

    bool isOpen();
};