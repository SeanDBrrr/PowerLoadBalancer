#include "IOfficeState.h"
#include "Button.h"
#pragma once
const int BUTTON_PIN = 13; // TODO NEEDS TO BE CHANGED

class OfficeKey : public IOfficeState
{
private:
    ToggleButton *_button;

public:
    OfficeKey();
    ~OfficeKey();

    bool isOpen();
};