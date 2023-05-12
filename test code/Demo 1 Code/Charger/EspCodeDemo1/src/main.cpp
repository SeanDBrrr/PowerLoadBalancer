// #include <Arduino.h>
// #include <string.h>
// #include "EspMQTTClient.h"
// #include "Button.h"
// #include "LCD.h"
// #include "Director_RFID.h"

// #define BUTTON_PIN 4
// #define CONNECTION_LED 2
// #define REQUEST_POWER_LED 15
// #define DIRECTOR_LED 26
// int counter = 0;
// const char *name = "Kiwy";
// const char *password = "aquamagic23";
// const char *mqtt_module = "Group4-Charger";
// const char *mqtt_topic = "group4/RequestPower";
// const char *mqtt_topic_receiver = "group4/SentPower";
// const char *broker_ip = "192.168.61.23";

// int dir = 1;
// int helper = 0;

// using namespace std;
// long previousTime = millis();
// Button button(BUTTON_PIN);

// LCD *lcd;
// EspMQTTClient client1(name, password, broker_ip, mqtt_module, 1883);
// Director_RFID *director;
// void setup()
// {
//   lcd = new LCD(22, 21);
//   director = new Director_RFID(5, 27, 2);
//   Serial.begin(115200);
//   button.begin();
//   pinMode(CONNECTION_LED, OUTPUT);
//   pinMode(REQUEST_POWER_LED, OUTPUT);
//   pinMode(DIRECTOR_LED, OUTPUT);
//   client1.enableHTTPWebUpdater();
//   client1.enableDebuggingMessages();
// }

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

// void loop()
// {
//   //client1.loop();
//   if (director->ReadRFID() == SUCCESS)
//   {
//     digitalWrite(DIRECTOR_LED, HIGH);
//     lcd->Display("DIRECTOR");
//   }
//   else
//   {
//     digitalWrite(DIRECTOR_LED, LOW);
//   }

//   if (button.singlePress())
//   {
//      lcd->Display("Start");
//     counter++;
//     Serial.println("Request " + String(counter));
//   }
// }

//============================================================TESTING RFID============================================================================

// Libraries
#include <Arduino.h>
#include <SPI.h>     //https://www.arduino.cc/en/reference/SPI
#include <MFRC522.h> //https://github.com/miguelbalboa/rfid

#include "Director_RFID.h"
uint32_t id;
Director_RFID *director;
void setup()
{
  director = new Director_RFID(SS_PIN, RST_PIN);
  director->begin();
  Serial.begin(115200);
}


void loop()
{
  id = director->getID();
  if (id != 0)
  {
    Serial.println(id);
  }
}