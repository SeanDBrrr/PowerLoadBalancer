/**
 * @file Button.cpp
 * @author Sean-David Brokke (4208501)
 * @brief
 * @version 0.1
 * @date 2023-5-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Button.h"

using namespace std;

Button::Button(int btn) : _buttonPin(btn), _state(0)
{
}

void Button::begin()
{
  pinMode(_buttonPin, INPUT_PULLUP);
}

/**
 * @brief Debouncer using  bit shifting
 *
 * @return uint16_t Value of button state.
 */
uint16_t Button::debounce()
{       
  _state = (_state << 1) | digitalRead(_buttonPin) | DEBOUNCE_DELAY; // A beautiful press will result in the 16bit int being 0xFF00, (0XFE00)
  return (_state);
}

/**
 * @brief Button hold press function
 *
 * @return true, as long as button is pressed.
 * @return false, as long as button is released.
 */
bool Button::pressed()
{
  return (debounce() == BUTTON_PRESSED);
}

/**
 * @brief Button toggle function
 * When the button is initially pressed it goes to the true state. 
 * When the button is pressed again after being switched to the true state it goes into the 
 * false state. When in the false state the button is pressed it swithces to true state, and vice-versa. 
 * @return true, when buttton is in true state.
 * @return false, when buttton is in false state.
 */
bool Button::toggle() {
  static bool toggle_state = false; // static variable to store toggle state
  if (pressed()) { // check if button is pressed
    toggle_state = !toggle_state; // toggle the toggle state
  }
  return toggle_state; // return the current toggle state
}

/**
 * @brief Button Single Press function
 * This function normally returns false, only when the button is pressed the singlePressFlag gets triggerd 
 * once to become true and right after, it becomes false again untill another press is detected and so on.
 * @return true , once when button press flag detected.
 * @return false, in all other cases.
 */
bool Button::singlePress() {
  static bool singlePressFlag = false;
  if (pressed()) {
    if (!singlePressFlag) {  // check if the button has been pressed for the first time
      singlePressFlag = true;
      return true;
    }
  } else {
    singlePressFlag = false;
  }
  return false;
}

Button::~Button()
{
}