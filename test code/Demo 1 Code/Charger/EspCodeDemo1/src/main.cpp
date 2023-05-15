#include <Arduino.h>
#include <string.h>
// #include "EspMQTTClient.h"
#include "Button.h"
#include "DirectorScanner.h"
#include "StationScreen.h"

#define BUTTON_PIN 4
#define CONNECTION_LED 2
#define REQUEST_POWER_LED 15
#define DIRECTOR_LED 26
int counter = 0;
// const char *name = "Kiwy";
// const char *password = "aquamagic23";
// const char *mqtt_module = "Group4-Charger";
// const char *mqtt_topic = "group4/RequestPower";
// const char *mqtt_topic_receiver = "group4/SentPower";
// const char *broker_ip = "192.168.61.23";

// int dir = 1;
// int helper = 0;

using namespace std;
// long previousTime = millis();
Button *button;
uint32_t id = 0;
DirectorScanner *director;
StationScreen *lcd;
bool check;
// EspMQTTClient client1(name, password, broker_ip, mqtt_module, 1883);
// Director_RFID *director;
void setup()
{
  button = new Button(BUTTON_PIN);
  lcd = new StationScreen(22, 21);
  director = new DirectorScanner(SS_PIN, RST_PIN);
  Serial.begin(115200);
  // button.begin();
  pinMode(CONNECTION_LED, OUTPUT);
  pinMode(REQUEST_POWER_LED, OUTPUT);
  pinMode(DIRECTOR_LED, OUTPUT);
  // client1.enableHTTPWebUpdater();
  // client1.enableDebuggingMessages();
}

// void onConnectionEstablished()
// {
//   client1.subscribe(mqtt_topic_receiver, [](const String &topic, const String &payload)
//                     {
//       if(payload == "11kW")
//       {
//         digitalWrite(CONNECTION_LED, HIGH);
//       }
//       else if(payload == "0kW")
//       {
//         digitalWrite(CONNECTION_LED, LOW);
//       } });
// }

void loop()
{

  // id = director->getID();
  // if (id > 0)
  // {
  //   lcd->display(String(id));
  // }

  button->debounce();
  id = director->getID();
  // // client1.loop();
  // // id = director->getID();

  // /// RFID

  // // Serial.println(id);
  // if (id > 0)
  // {
  //   lcd->display(String(id));
  // }

  // // PRESS
  if (button->pressed())
  {
    if (id > 0)
    {
      lcd->display(String(id));
    }
    digitalWrite(REQUEST_POWER_LED, HIGH);
  }
  else
  {
    digitalWrite(REQUEST_POWER_LED, LOW);
  }

  // TOGGLE
  if (button->toggle())
  {
    if (id > 0)
    {
      lcd->display(String(id));
    }
    digitalWrite(DIRECTOR_LED, HIGH);
  }
  else
  {
    digitalWrite(DIRECTOR_LED, LOW);
  }

  // // SINGLE PRESS
  // if (button->singlePress())
  // {
  //   lcd->display(String(counter));
  //   counter++;
  // }
}

//============================================================TESTING RFID============================================================================

// MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// void setup() {
// 	Serial.begin(115200);		// Initialize serial communications with the PC
// 	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
// 	SPI.begin();			// Init SPI bus
// 	mfrc522.PCD_Init();		// Init MFRC522
// 	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
// 	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
// 	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
// }

// void loop() {
// 	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
// 	if ( ! mfrc522.PICC_IsNewCardPresent()) {
// 		return;
// 	}

// 	// Select one of the cards
// 	if ( ! mfrc522.PICC_ReadCardSerial()) {
// 		return;
// 	}

// 	// Dump debug info about the card; PICC_HaltA() is automatically called
// 	mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
// }
