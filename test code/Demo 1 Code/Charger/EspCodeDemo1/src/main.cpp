#include <Arduino.h>
#include <string.h>
#include "EspMQTTClient.h"
#include "Button.h"

#define BUTTON_PIN 4
#define CONNECTION_LED 2
#define REQUEST_POWER_LED 5
int counter = 0;
const char *name = "Kiwy";
const char *password = "aquamagic23";
const char *mqtt_module = "Group4-Charger";
const char *mqtt_topic = "group4/RequestPower";
const char *mqtt_topic_receiver = "group4/SentPower";
const char *broker_ip = "192.168.61.23";

int dir = 1;
int helper = 0;

using namespace std;
long previousTime = millis();
Button button(BUTTON_PIN); 

EspMQTTClient client1(name, password, broker_ip, mqtt_module, 1883);

void setup()
{
  Serial.begin(115200);
  button.begin();
  pinMode(CONNECTION_LED,OUTPUT);
  pinMode(REQUEST_POWER_LED,OUTPUT);
  client1.enableHTTPWebUpdater();
  client1.enableDebuggingMessages();
}

void onConnectionEstablished()
{
  client1.subscribe(mqtt_topic_receiver, [](const String & topic, const String & payload) {
      if(payload == "11kW")
      {
        digitalWrite(CONNECTION_LED, HIGH);
      }
      else if(payload == "0kW")
      {
        digitalWrite(CONNECTION_LED, LOW);
      }
  });
}


bool Debounce() {
  static uint16_t state = 0;
  state = (state<<1) | digitalRead(BUTTON_PIN) | 0xfe00;
  Serial.println(state);
  return (state == 0xff00);
}

void loop()
{
  client1.loop();
  
  // if (button.Pressed())
  // {
  //   if(helper == 0)
  //   {
  //    // digitalWrite(REQUEST_POWER_LED, HIGH);
  //     Serial.println("Pressed");
  //     if(dir == 1)
  //     {
  //       client1.publish(mqtt_topic, "requestPower");
  //     }
  //     else if(dir == -1)
  //     {
  //       client1.publish(mqtt_topic, "requestStop");
  //     }
  //     dir *= -1;
  //     helper = 1;
  //   }
  // }

  //button.Toggled();
   //Serial.println(button.Debounce());

  if (button.singlePress())
  {
    counter++;
    Serial.println("Request " + String(counter));
 }
  // else
  // {
  // }
  //  if (button.Pressed())
  // {
  //   digitalWrite(REQUEST_POWER_LED, HIGH);

  // }
  // else
  // {
  //   digitalWrite(REQUEST_POWER_LED, LOW);
  // }

}