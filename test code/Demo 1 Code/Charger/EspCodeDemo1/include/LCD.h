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
#include <LiquidCrystal_I2C.h>

class LCD
{
private:
    int SCL;
    int SDA;
    LiquidCrystal_I2C lcd;

public:
    LCD(int, int);
    ~LCD();

    int Display(String);
};
