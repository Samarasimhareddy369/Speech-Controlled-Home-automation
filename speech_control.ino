#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID "mani"//Your wifi name
#define WIFI_PASS "manikanta@1839"//your wifi password

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "RLR_AUTOMATION" //Your adafruit name
#define MQTT_PASS "d58b2b54da0e422d91f8adb3b876077a" //Your adafruit AIO key

int led = D8;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

Adafruit_MQTT_Subscribe inaugurateastronicsthreepoint = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/inaugurateastronicsthreepoint");
//Adafruit_MQTT_Publish LightsStatus = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/LightsStatus");


void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);


  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi>");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  digitalWrite(LED_BUILTIN, LOW);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(">");
    delay(50);
  }

  Serial.println("OK!");

  //Subscribe to the Lights topic
  mqtt.subscribe(&inaugurateastronicsthreepoint);

  pinMode(led, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(led, LOW);

}

void loop()
{
  //Connect/Reconnect to MQTT
  MQTT_connect();

  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &inaugurateastronicsthreepoint)
    {
      //Print the new value to the serial monitor
      Serial.print("inaugurateastronicsthreepoint: ");
      Serial.println((char*) inaugurateastronicsthreepoint.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) inaugurateastronicsthreepoint.lastread, "O"))
      {
        //active low logic
        digitalWrite(led, HIGH);
        //LightsStatus.publish("ON");
      }
      else if (!strcmp((char*) inaugurateastronicsthreepoint.lastread, "OFF"))
      {
        digitalWrite(led, LOW);
        //LightsStatus.publish("OFF");

      
      //else
      //{
        //LightsStatus.publish("ERROR");
      //}
      }
    else
    {
      //LightsStatus.publish("ERROR");
    }
  }
  //  if (!mqtt.ping())
  //  {
  //    mqtt.disconnect();
  //  }
}
}


void MQTT_connect()
{

  //  // Stop if already connected
  if (mqtt.connected() && mqtt.ping())
  {
    //    mqtt.disconnect();
    return;
  }

  int8_t ret;

  mqtt.disconnect();

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0)
    {
      ESP.reset();
    }
  }
  Serial.println("MQTT Connected!");
}
