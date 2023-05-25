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
private:
  int _buttonPin;
  uint16_t _state;
  const uint16_t _RELEASED = 0xFFFF;
  const uint16_t _DEBOUNCE_MAX_TARGET = 0xFFE0;
  const uint16_t _DELAY_TARGET = 0xFFFD;

public:
  Button(int);
  // void read();
  void debounce();
  bool pressed();
  bool toggle();
  bool singlePress();
};