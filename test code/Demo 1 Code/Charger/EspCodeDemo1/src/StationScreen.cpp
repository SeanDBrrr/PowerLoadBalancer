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
StationScreen::StationScreen(int scl, int sda) : _scl(scl),
                                                 _sda(sda)

{
    lcd = new LiquidCrystal_I2C(SS_ADDRESS, SS_COLUMNS, SS_ROWS);
    lcd->init();
    lcd->backlight();
    lcd->setCursor(0, 0);
}


/**
 * @brief Function for displaying StationScreen
 *
 * @param message Input message to be displayed on StationScreen
 * @return int for future unit testing implementation
 */
void StationScreen::display(String message)
{
    lcd->clear();
    lcd->print(message);
}

StationScreen::~StationScreen()
{
    delete lcd;
}
