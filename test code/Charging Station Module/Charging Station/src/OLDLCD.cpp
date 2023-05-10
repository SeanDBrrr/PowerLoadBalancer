#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "IDisplay.h"

class LCD : public IDisplay
{

private:
    LiquidCrystal_I2C &_lcd;
    uint8_t lcd_Addr, lcd_cols, lcd_rows;

public:
    LCD(uint8_t &lcd_Addr2, uint8_t &lcd_cols2, uint8_t &lcd_rows2)
        : lcd_Addr(lcd_Addr2), lcd_cols(lcd_cols2), lcd_rows(lcd_cols2)
    {
        LiquidCrystal_I2C _lcd(lcd_Addr, lcd_cols, lcd_rows);
    }
    ~LCD() {}

    void display(uint8_t str)
    {
        _lcd.write(str);
    }
};
