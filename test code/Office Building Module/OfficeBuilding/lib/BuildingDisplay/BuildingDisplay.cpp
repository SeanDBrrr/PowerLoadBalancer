#include "LiquidCrystal_I2C.h"
#include "BuildingDisplay.h"

constexpr uint8_t SS_ADDRESS = 0x27;
constexpr uint8_t SS_COLUMNS = 16;
constexpr uint8_t SS_ROWS = 2;

LiquidCrystal_I2C lcd(SS_ADDRESS, SS_COLUMNS, SS_ROWS);

BuildingDisplay::BuildingDisplay(int scl, int sda) : _scl{scl}, _sda{sda}
{
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
}

void 
BuildingDisplay::display(String message)
{
    lcd.clear();
    lcd.print(message);
}