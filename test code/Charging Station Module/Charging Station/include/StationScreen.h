/**
 * @file StationScreen.h
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

#define SS_ADDRESS 0x27
#define SS_COLUMNS 16
#define SS_ROWS 2
class StationScreen
{
private:
    int _scl;
    int _sda;
    LiquidCrystal_I2C *lcd;

public:
    StationScreen(int, int);
    ~StationScreen();
    void display(String);
};
