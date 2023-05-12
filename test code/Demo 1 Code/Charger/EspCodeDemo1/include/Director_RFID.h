/**
 * @file Director_RFID.h
 * @author Sean-David Brokke (460282)
 * @brief
 * @version 0.1
 * @date 2023-01-20
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Arduino.h"
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 27

typedef enum
{
    IDLE,
    CARD_DETECTED,
    RESET
} States;

class Director_RFID
{
private:
    int _ssPin;
    int _rstPin;
    uint32_t _id = 0;
    MFRC522 *_rfid;
    MFRC522::MIFARE_Key _key;
    byte _nuidPICC[4] = {0, 0, 0, 0};
    uint32_t assembleID(byte *, byte);

public:
    Director_RFID(int, int);
    uint32_t getID();
    void begin();
    ~Director_RFID();
};
