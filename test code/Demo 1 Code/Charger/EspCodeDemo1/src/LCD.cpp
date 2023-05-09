/**
 * @file LCD.cpp
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
#include "LCD.h"
#include <iostream>

using namespace std;

/**
 * @brief Construct a new LCD::LCD object
 *
 * @param scl
 * @param sda
 */
LCD::LCD(int scl, int sda)
    : SCL(scl),
      SDA(sda),
      lcd(0x27, 16, 2)
{
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
}

LCD::~LCD()
{
}

/**
 * @brief Function for displaying LCD
 *
 * @param message Input message to be displayed on LCD
 * @return int for future unit testing implementation
 */
int LCD::Display(String message)
{
    lcd.clear();
    lcd.print(message);
    return 0;
}