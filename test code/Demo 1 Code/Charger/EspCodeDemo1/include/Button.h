/**
 * @file Button.h
 * @author Sean-David Brokke (4208501)
 * @brief
 * @version 0.1
 * @date 2023-04-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <Arduino.h>

#define BUTTON_PRESSED 0xFF00
#define BUTTON_RELEASED 0xFE01
#define DEBOUNCE_DELAY 0xE000

class Button
{
private:
  int _buttonPin;
  uint16_t _state;

public:
  Button(int);
  ~Button();
  void begin();
  uint16_t debounce();
  bool pressed();
  bool toggle();
  bool singlePress();
};

// class Button
// {
// private:
//   uint8_t btn;
//   uint16_t state;

// public:
//   void begin(uint8_t button)
//   {
//     btn = button;
//     state = 0;
//     pinMode(btn, INPUT_PULLUP);
//   }
//   bool Debounce()
//   {
//     state = (state << 1) | digitalRead(btn) | 0xfe00;
//     Serial.println(state);
//     return (state == 0xff00);
//   }
// };