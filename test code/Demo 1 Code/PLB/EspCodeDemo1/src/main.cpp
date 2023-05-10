#include <Arduino.h>
#include <string.h>
#include <EspMQTTClient.h>
 
const char *name = "Kiwy";
const char *password = "aquamagic23";
const char *mqtt_module = "Group4-PLB";
const char *mqtt_topic_1 = "group4/SentPower1";
const char *mqtt_topic_2 = "group4/SentPower2";
const char *mqtt_topic_gate = "group4/gateUpdate";
const char *mqtt_topic_receiver = "group4/RequestPower";
const char *broker_ip = "192.168.193.23";

int gateNumber = 2;

const int CONNECTION_LED = 2;
long previousTime = millis();

EspMQTTClient client1(name, password, broker_ip, mqtt_module, 1883);

void setup()
{
  Serial.begin(115200);
  pinMode(CONNECTION_LED,OUTPUT);
  client1.enableHTTPWebUpdater();
  client1.enableDebuggingMessages();
}

void onConnectionEstablished()
{
  digitalWrite(CONNECTION_LED, HIGH);
  client1.subscribe(mqtt_topic_receiver, [](const String & topic, const String & payload) {
      if(payload == "requestPower:Director:charger1")
      {
        if(gateNumber == 2)
        {
          client1.publish(mqtt_topic_1, "Director:11kW", 0);
          gateNumber--;
        }
        else
        {
          client1.publish(mqtt_topic_1, "Director:5kW", 0);
          gateNumber--;
        }
      }
      else if(payload == "requestPower:User:charger1")
      {
        if(gateNumber == 1)
        {
          client1.publish(mqtt_topic_1, "User:11kW", 0);
          gateNumber--;
        }
        else if(gateNumber == 1)
        {
          client1.publish(mqtt_topic_1, "User:5kW", 0);
          gateNumber--;
        }
      }
      else if(payload == "requestPower:Director:charger2")
      {
        if(gateNumber == 2)
        {
          client1.publish(mqtt_topic_2, "Director:11kW", 0);
          gateNumber--;
        }
        else if(gateNumber == 1)
        {
          client1.publish(mqtt_topic_2, "Director:5kW", 0);
          gateNumber--;
        }
      }
      else if(payload == "requestPower:User:charger2")
      {
        if(gateNumber == 2)
        {
          client1.publish(mqtt_topic_2, "User:11kW", 0);
          gateNumber--;
        }
        else if(gateNumber == 1)
        {
          client1.publish(mqtt_topic_2, "User:5kW", 0);
          gateNumber--;
        }
      }
  });

  client1.subscribe(mqtt_topic_gate, [](const String & topic, const String & payload) {
    gateNumber++;
  });
}

void loop()
{
  client1.loop();

  if(!client1.isConnected())
  {
    digitalWrite(CONNECTION_LED, LOW);
  }
}