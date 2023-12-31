#include "IStart.h"
#include "Button.h"
#include "StartButton.h"
#include <iostream>

const int BUTTON_PIN = 13; // TODO NEEDS TO BE CHANGED

/// @brief Construct a new StartButton::StartButton object
StartButton::StartButton()
{
    _button = new SinglePressButton(BUTTON_PIN);
}

/// @brief Destruct a StartButton::StartButton object
StartButton::~StartButton()
{
    delete _button;
}

/// @brief If charging station is Started.
/// @return True or false
bool StartButton::isStarted()
{
    return _button->pressed();
}