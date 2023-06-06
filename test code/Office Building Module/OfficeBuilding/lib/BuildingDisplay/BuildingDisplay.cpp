#include "BuildingDisplay.h"

constexpr uint8_t SS_ADDRESS = 0x27;
constexpr uint8_t SS_COLUMNS = 16;
constexpr uint8_t SS_ROWS = 2;

BuildingDisplay::BuildingDisplay(int scl, int sda): 
    _scl{scl}, _sda{sda}, _lcd{LiquidCrystal_I2C(SS_ADDRESS, SS_COLUMNS, SS_ROWS)}
{
    _lcd.init();
    _lcd.backlight();
    _lcd.setCursor(0, 0);
}

void BuildingDisplay::display(String msg1, String msg2)
{
    _lcd.clear();
    _lcd.setCursor(0,0);
    _lcd.print(msg1);
    _lcd.setCursor(0,1);
    _lcd.print(msg2);
}