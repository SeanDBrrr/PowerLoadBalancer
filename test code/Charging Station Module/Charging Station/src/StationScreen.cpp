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
#include <LiquidCrystal_I2C.h>
#include "IDisplay.h"
#include <iostream>

using namespace std;

/**
 * @brief Construct a new StationScreen::StationScreen object
 *
 * @param scl
 * @param sda
 */
StationScreen::StationScreen(int scl, int sda)
    : SCL(scl),
      SDA(sda),
      lcd(0x27, 16, 2)
{
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
}

StationScreen::~StationScreen()
{
}

/**
 * @brief Function for displaying StationScreen
 *
 * @param message Input message to be displayed on StationScreen
 * @return int for future unit testing implementation
 */
int StationScreen::Display(String message)
{
    lcd.clear();
    lcd.print(message);
    return 0;
}