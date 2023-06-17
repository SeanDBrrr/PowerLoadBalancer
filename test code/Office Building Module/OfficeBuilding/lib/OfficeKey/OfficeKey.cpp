#include "IOfficeState.h"
#include "Button.h"
#include "OfficeKey.h"
#include <iostream>


/// @brief Construct a new OfficeKey::StartButton object
OfficeKey::OfficeKey()
{
    _button = new ToggleButton(BUTTON_PIN);
}

/// @brief Destruct a OfficeKey::OfficeKey object
OfficeKey::~OfficeKey()
{
    delete _button;
}

/// @brief If charging station is Started.
/// @return True or false
bool OfficeKey::isOpen()
{
    return _button->pressed();
}