#include "IOfficeState.h"
#include "Button.h"
#include "BuildingStateButton.h"
#include <iostream>


/// @brief Construct a new BuildingStateButton::StartButton object
BuildingStateButton::BuildingStateButton()
{
    _button = new SinglePressButton(BUTTON_PIN);
}

/// @brief Destruct a BuildingStateButton::BuildingStateButton object
BuildingStateButton::~BuildingStateButton()
{
    delete _button;
}

/// @brief If charging station is Started.
/// @return True or false
bool BuildingStateButton::isOpen()
{
    return _button->pressed();
}