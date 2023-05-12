/**
 * @file LCD.h
 * @authors
 * Sean-David Brokke (4208501)
 * Andre Sanao (4425553)
 * @brief
 * @version 0.1
 * @date 2023-01-20
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Arduino.h"
#include "IDisplay.h"
#include <LiquidCrystal_I2C.h>

class StationScreen : public IDisplay
{
private:
    int _SCL;
    int _SDA;
    LiquidCrystal_I2C *_lcd;

public:
    StationScreen(int, int);
    ~StationScreen();

    int Display(String);
};
