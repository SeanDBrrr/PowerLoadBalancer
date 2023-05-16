/**
 * @file Director_RFID.cpp
 * @author Sean-David Brokke (4208501)
 * @brief
 * @version 0.1
 * @date 2023-01-20
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "DirectorScanner.h"
#include <iostream>

using namespace std;

/**
 * @brief DirectorScanner constructor
 * Constructor must be called inside void setup during object creation using new operator!!
 * @param SS
 * @param RST
 */
DirectorScanner::DirectorScanner(int SS, int RST) : _ssPin(SS),
                                                    _rstPin(RST)

{
  _rfid = new MFRC522(_ssPin, _rstPin);
  SPI.begin();
  _rfid->PCD_Init();
  delay(4); // Optional delay. Some board do need more time after init to be ready, see Readme
}

/**
 * @brief Function to assemble all the bits of the ID to be stored in one single variable.
 *
 * @param buffer
 * @param bufferSize
 * @return ID in uint32_t
 */
uint32_t DirectorScanner::assembleID(byte *buffer, byte bufferSize)
{
  uint32_t value = 0;
  for (byte i = bufferSize; i > 0; i--)
  {
    value |= static_cast<uint32_t>(buffer[i - 1]) << (8 * (bufferSize - i));
  }
  return value;
}

/**
 * @brief This function retrives the RFID of the director that has scanned his/her card
 *
 * @return Director's ID, else 0 when no card is detected. (uint32_t)
 */
uint32_t DirectorScanner::getID()
{
  /* function readRFID */
  ////Read RFID card
  uint32_t id = 0;
  for (byte i = 0; i < 6; i++)
  {
    _key.keyByte[i] = 0xFF;
  }
  // Look for new 1 cards
  if (!_rfid->PICC_IsNewCardPresent())
  {
    // Serial.println("Inside NOT 'IsNewCardPresent'");
    return id;
  }

  // Verify if the NUID has been readed
  if (!_rfid->PICC_ReadCardSerial())
  {
    // Serial.println("Inside NOT 'ReadCardSerial()'");
    return id;
  }
  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++)
  {
    _nuidPICC[i] = _rfid->uid.uidByte[i];
  }
  id = assembleID(_rfid->uid.uidByte, _rfid->uid.size);
  // Halt PICC
  _rfid->PICC_HaltA();
  // Stop encryption on PCD
  _rfid->PCD_StopCrypto1();

  return id;
}

DirectorScanner::~DirectorScanner()
{
  delete _rfid;
}
