/**
 * @file StationScreen.cpp
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
#include "StationScreen.h"
#include <iostream>

using namespace std;

/**
 * @brief Construct a new StationScreen::StationScreen object
 *
 * @param scl
 * @param sda
 */
StationScreen::StationScreen(int scl, int sda)
    : _SCL(scl),
      _SDA(sda)
{
    _lcd = new LiquidCrystal_I2C(0x27, 16, 2);
    _lcd->init();
    _lcd->backlight();
    _lcd->setCursor(0, 0);
}

StationScreen::~StationScreen()
{
    delete _lcd;
}

/**
 * @brief Function for displaying StationScreen
 *
 * @param message Input message to be displayed on StationScreen
 * @return int for future unit testing implementation
 */
int StationScreen::Display(String message)
{
    _lcd->clear();
    _lcd->print(message);
    return 0;
}