#include "IPlug.h"
#include "Button.h"
#include "PlugButton.h"
#include <iostream>

#define BUTTON_PIN 4 // TODO NEEDS TO BE CHANGED

/// @brief Construct a new PlugButton::PlugButton object
PlugButton::PlugButton()
{
    _button = new SinglePressButton(BUTTON_PIN); // DOUBLE CHECK IF THIS WORKS OUTSIDE OF VOID SETUP
}

/// @brief Destruct a PlugButton::PlugButton object
PlugButton::~PlugButton()
{
    delete _button;
}

/// @brief If charging station is plugged.
/// @return True or false
bool PlugButton::isPlugged()
{
    return _button->pressed();
}