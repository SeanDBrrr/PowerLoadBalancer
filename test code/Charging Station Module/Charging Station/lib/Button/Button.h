/**
 * @file Button.cpp
 * @author Sean-David Brokke (4208501)
 * @brief
 * @version 1.1
 * @date 2023-5-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <Arduino.h>

class Button
{
protected:
  int _buttonPin;
  uint16_t _state;
  const uint16_t _RELEASED = 0xFFFF;
  const uint16_t _DEBOUNCE_MAX_TARGET = 0xFFE0;
  const uint16_t _DELAY_TARGET = 0xFFFD;
  bool singlePress();
  uint16_t debounce();

public:
  Button(int);
};

class PushHoldButton : public Button
{
public:
  PushHoldButton(int pin) : Button(pin) {}
  bool pressed();
};

class ToggleButton : public Button
{
public:
  ToggleButton(int pin) : Button(pin) {}
  bool pressed();
};

class SinglePressButton : public Button
{
public:
  SinglePressButton(int pin) : Button(pin) {}
  bool pressed();
};
