#include <Arduino.h>
#include <arduino-timer.h>
#include <ArduinoJson.h>
#include "PinHandler.h"
#include "Tools.h"
#include <WiFi.h>
#include <MQTT.h>
#include <Button2.h>
#include "defines.h"

#define MQTT_INBOUND "rlg/cmd/"
#define MQTT_OUTBOUND "rlg/evt/"
#define MQTT_BROKER "192.168.232.253"
#define MY_ID "ag22"

#define NUMBER_OF_TIMERS_VARS 10

// forward declarations
// void init_wifi();
void connectToMQTT();
void onMessageReceived(String &topic, String &payload);
bool every_25ms(void *argument);
bool every_second(void *argument);
void proc_commands(String &topic, String &payload);
void button_handler(Button2 &b);

// network setup
const char *ssid = WIFI_SSID;
const char *password = WIFI_PSK;
unsigned long previousMillis = 0;
unsigned long check_wifi_interval = 30000;
WiFiClient wifiClient;
MQTTClient mqttclient;
// network setup

auto timer = timer_create_default();
// int gpio_assignment[NUMBER_OF_ALL_PINS] = {13, 14, 18, 19, 21, 22, 23, 25, 26, 27}; //   2, 4, 5, 18, 19, 21, 22, 23, 25, 26
int timers[NUMBER_OF_TIMERS_VARS];
int vars[NUMBER_OF_TIMERS_VARS];

Button2 btn01;
Button2 btn02;

PinHandler pinHandler;

void connectToWiFi()
{
  int TryCount = 0;
  WiFi.mode(WIFI_STA);
  debugln("Trying to connect to wifi.");
  debug(ssid);

  while (WiFi.status() != WL_CONNECTED)
  {
    TryCount++;
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    debug(".");
    vTaskDelay(4000);
    if (TryCount == 10)
    {
      debugln("Couldn't connect to wifi. Tried 10 times. Restarting...");
      ESP.restart();
    }
  }
  debugln("");
  debugln("WiFi connected");
  debugln("IP address: ");
  debugln(WiFi.localIP());
  debug("RSSI: ");
  debug(WiFi.RSSI());
  debugln(Tools::get_wifi_quality(WiFi.RSSI()));

#ifdef DEBUG_MODE
  WiFi.onEvent(Tools::log_wifi_event);
#endif

} // void connectToWiFi()

void onMessageReceived(String &topic, String &payload)
{
  debugln("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void connectToMQTT()
{
  // note: https://forum.arduino.cc/t/once-wificlient-fails-it-never-connects-again/1045456/6
  // and this: https://forum.arduino.cc/t/once-wificlient-fails-it-never-connects-again/1045456/2
  debug("checking wifi...");
  if (WiFi.status() != WL_CONNECTED)
  {
    debugln("no wifi - no mqtt");
    vTaskDelay(1000);
    return;
  }

  byte mac[5];
  WiFi.macAddress(mac);
  String clientID = String(mac[0]) + String(mac[4]); // use mac address to create clientID

  debugln("\nconnecting to MQTT...");
  while (!mqttclient.connect(clientID.c_str()))
  {
    debug(".");
    vTaskDelay(250);
  }

  debugln("MQTT connected");
  mqttclient.onMessage(onMessageReceived);
  // mqttclient.onMessage(proc_commands);
  mqttclient.subscribe(MQTT_INBOUND MY_ID "/#");
}

bool every_25ms(void *argument)
{
  pinHandler.loop();
  return true;
}

bool every_second(void *argument)
{
  // recalculate timers
  // for (int i = 0; i < NUMBER_OF_TIMERS_VARS; i++)
  // {
  //   if (timers[i] > 0)
  //     timers[i] = max(timers[i] - 1000, 0);
  // }
  debug("");
  return true;
}

void proc_commands(String &topic, String &payload)
{
  debug("Message arrived in topic: " + topic + " with Payload: " + payload);

  // last part of topic is the command
  // char *cmd = strrchr(topic, '/');
  int pos_of_slash = topic.lastIndexOf("/");
  String cmd = topic.substring(pos_of_slash);

  StaticJsonDocument<1024> incoming;
  DeserializationError jsonError = deserializeJson(incoming, payload);

  // Test if parsing succeeds.
  if (jsonError)
  {
    debug("deserializeJson() failed: ");
    debugln(jsonError.f_str());
    return;
  }

  if (cmd.equals("/visual") || cmd.equals("/acoustic"))
  {
    pinHandler.parse_incoming(incoming);
  }
  else if (cmd.equals("/timers"))
  {
    // int value = incoming.
  }
  incoming.clear();
}

void button_handler(Button2 &b)
{
  debugln("entry button handler");

  debugln("exit button handler");
}

void setup()
{
  Serial.begin(115200);

  connectToWiFi();
  mqttclient.begin(MQTT_BROKER, wifiClient);
  connectToMQTT();

  pinHandler.begin();

  btn01.begin(BTN01);
  btn01.setID(1);
  btn01.setPressedHandler(button_handler);
  btn01.setReleasedHandler(button_handler);

  btn02.begin(BTN02);
  btn02.setID(2);
  btn02.setPressedHandler(button_handler);
  btn02.setReleasedHandler(button_handler);

  timer.every(25, every_25ms);
  // timer.every(1000, every_xsecond);
  // timer.every(30000, every_30s);
}

void loop()
{
  // see https://github.com/256dpi/arduino-mqtt/blob/master/examples/ESP32DevelopmentBoard/ESP32DevelopmentBoard.ino
  mqttclient.loop();
  delay(10); // <- fixes some issues with WiFi stability
  if (!mqttclient.connected())
  {
    connectToMQTT();
  }
  timer.tick();
  btn01.loop();
  btn02.loop();
}