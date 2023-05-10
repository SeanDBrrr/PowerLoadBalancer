/**
 * @file Director_RFID.cpp
 * @authors
 *  Sean-David Brokke (4208501)
 * Luka Aerts (4202317)
 * @brief 
 * @version 0.1
 * @date 2023-01-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "Director_RFID.h"
#include <iostream>

using namespace std;

/**
 * @brief Construct a new Gate:: Gate object
 * 
 * @param SDA 
 * @param RST 
 * @param ledpin_ 
 */
Director_RFID::Director_RFID(int SDA, int RST, int ledpin_) : state(IDLE), previousTime(0), ledPin(ledpin_), rfid(MFRC522(SDA, RST))
{
  SPI.begin();
  rfid.PCD_Init();
  rfid.PCD_DumpVersionToSerial();
  pinMode(ledPin, OUTPUT);
}

/**
 * @brief This function checks if a RFID card has been detected or not
 * 
 * @return int card succesfully detected or Error_ for card not detected
 */
int Director_RFID::ReadRFID()
{ 
  //Read RFID card
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }

  // Look for new 1 cards
  if (!rfid.PICC_IsNewCardPresent())
  {
    return ERROR_;
  }

  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
  {
    return SUCCESS;
  }
  return NO_CARD_DETECTED;
}

/**
 * @brief Simulates gate in open state
 * 
 */
void Director_RFID::OpenGate()
{
  digitalWrite(ledPin, HIGH);
}

/**
 * @brief Simulates gate in closed state
 * 
 */
void Director_RFID::CloseGate()
{
  digitalWrite(ledPin, LOW);
}

/**
 * @brief 
 * This function has the sequence for simulating a gate with an LED, 
 * with the help of RFID reader
 * @return int State of the gate
 */
int Director_RFID::GateState()
{
  int gateState = 0;
  switch (state)
  {
  case IDLE:
    if (ReadRFID() == SUCCESS)
    {
      gateState = OPEN;
      state = CARD_DETECTED;
    }
    previousTime = millis();
    break;
  case CARD_DETECTED:
    gateState = CLOSED;
    OpenGate();
    if (millis() - previousTime >= INTERVAL_GATE)
    {
      state = RESET;    
    }
    break;
  case RESET:
    CloseGate();
    state = IDLE;
    break;
  }

  return gateState;
}

Director_RFID::~Director_RFID()
{
}
