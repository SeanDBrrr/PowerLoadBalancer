/**
 * @file Button.cpp
 * @author Sean-David Brokke (4208501)
 * @brief 
 * @version 0.1
 * @date 2023-04-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "Button.h"

using namespace std;


Button::Button(int btnPin): _buttonPin(btnPin), _state(0)
{
    pinMode(_buttonPin, INPUT_PULLUP);
}

Button::~Button()	
{
}



/**
 * @brief Debouncer using  bit shifting
 * 
 * @return uint16_t Value of button state.
 */
uint16_t Button::Debounce()
{
  _state = 0; //static means there is 1 variable available for everyone(reserved only for this function)
  _state = (_state << 1) | digitalRead(_buttonPin) | BUTTON_PRESSED; // A beautiful press will result in the 16bit int being 0xFF00, (0XFE00)
  return _state;  // if there is bouncig result will be something like: 01010 1100 01010 01011 
}

/**
 * @brief Function to check if button pressed
 * 
 * @return true 
 * @return false 
 */
bool Button::Pressed()
{
  return (Debounce() == BUTTON_PRESSED);
  Serial.println(_state);
}
