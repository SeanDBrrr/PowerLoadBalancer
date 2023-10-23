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

/**
 * @brief Construct a new StationScreen::StationScreen object
 *
 * @param scl
 * @param sda
 */
StationScreen::StationScreen(
    int scl,
    int sda
    )
    : _scl(scl),
      _sda(sda),
      _lcd(LiquidCrystal_I2C(SS_ADDRESS, SS_COLUMNS, SS_ROWS))
{
    _lcd.init();
    _lcd.backlight();
    _lcd.setCursor(0, 0);
}

/**
 * @brief Function for displaying StationScreen
 *
 * @param message Input message to be displayed on StationScreen
 * @return int for future unit testing implementation
 */
void StationScreen::display(String msg1, String msg2)
{
    _lcd.clear();
    _lcd.setCursor(0,0);
    _lcd.print(msg1);
    _lcd.setCursor(0,1);
    _lcd.print(msg2);
}
