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
#include "Director_RFID.h"
#include <iostream>

using namespace std;

/**
 * @brief Construct a new Gate:: Gate object
 *
 * @param SS
 * @param RST
 */
Director_RFID::Director_RFID(int SS, int RST) : _ssPin(SS),
                                               _rstPin(RST)
                                                
{
  _rfid = new MFRC522(_ssPin,_rstPin);
    SPI.begin();
  _rfid->PCD_Init();
}

uint32_t Director_RFID::assembleID(byte *buffer, byte bufferSize)
{
  uint32_t value = 0;
  for (byte i = bufferSize; i > 0; i--)
  {
    value |= static_cast<uint32_t>(buffer[i - 1]) << (8 * (bufferSize - i));
  }
  return value;
}

/**
 * @brief This function checks if a RFID card has been detected or not
 *
 * @return int card succesfully detected or Error_ for card not detected
 */
uint32_t Director_RFID::getID()
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
    return id;
  }

  // Verify if the NUID has been readed
  if (!_rfid->PICC_ReadCardSerial())
  {
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

Director_RFID::~Director_RFID()
{
  delete _rfid;
}
