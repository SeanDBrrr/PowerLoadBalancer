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

#define RELEASED 0xFFFF
#define DEBOUNCE_MAX_TARGET 0xFFE0
#define DELAY_TARGET 0xFFFD
#define PRESSED(x) ((x) < DELAY_TARGET)
//#define SINGLE_PRESS_LOWEST 0xFFFD
#define SINGLE_PRESS(x) ((x) < RELEASED && (x) > (DELAY_TARGET))

class Button
{
private:
  int _buttonPin;
  uint16_t _state;

public:
  Button(int);
  // void read();
  // void begin();
  void debounce();
  bool pressed();
  bool toggle();
  bool singlePress();
    ~Button();
};

