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

#define BUTTON_PRESSED 0xE000

class Button 
{
  private:
    int _buttonPin;
    uint16_t _state;

  public:
    Button(int);
    ~Button();
    uint16_t Debounce();
    bool Pressed();
};