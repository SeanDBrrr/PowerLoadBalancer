/**
 * @file Director_RFID.h
 * @authors
 *  Sean-David Brokke (460282)
 *  Luka Aerts (4202317)
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

#define ERROR_ -1
#define SUCCESS 1
#define NO_CARD_DETECTED 0
#define OPEN 1
#define CLOSED 0
#define INTERVAL_GATE 5000

typedef enum 
{
    IDLE,
    CARD_DETECTED,
    RESET
}States;

class Director_RFID
{
private:
    int ledPin;
    unsigned long previousTime;
    MFRC522::MIFARE_Key key;
    MFRC522 rfid;
    States state;
public:
    Director_RFID(int, int, int);
    ~Director_RFID();
    int ReadRFID();
    int GateState();
    void OpenGate();
    void CloseGate();
};
