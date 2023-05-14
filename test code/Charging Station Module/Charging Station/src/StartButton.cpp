#include "IStart.h"
#include "Button.h"
#include "StartButton.h"
#include <iostream>

#define BUTTON_PIN 3 // TODO NEEDS TO BE CHANGED

using namespace std;

/// @brief Construct a new StartButton::StartButton object
StartButton::StartButton()
{
    _button = new Button(BUTTON_PIN);
    _button->begin();
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
    return _button->toggle();
}