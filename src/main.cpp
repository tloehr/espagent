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
void init_mqtt();
void connectToMQTT();
void reconnect_mqtt();
void onMessageReceived(String &topic, String &payload);
bool every_25ms(void *argument);
bool every_second(void *argument);
bool every_30s(void *argument);
void proc_commands(char *topic, byte *payload, unsigned int length);
void handleButton(Button2 &b);
void WiFiEvent(WiFiEvent_t event);

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

// PinHandler pinHandler = PinHandler();

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

  WiFi.onEvent(WiFiEvent);
} // void connectToWiFi()

void WiFiEvent(WiFiEvent_t event)
{
  debug("[WiFi-event] event:");
  debugln(event);

  switch (event)
  {
  case SYSTEM_EVENT_WIFI_READY:
    debugln("WiFi interface ready");
    break;
  case SYSTEM_EVENT_SCAN_DONE:
    debugln("Completed scan for access points");
    break;
  case SYSTEM_EVENT_STA_START:
    debugln("WiFi client started");
    break;
  case SYSTEM_EVENT_STA_STOP:
    debugln("WiFi clients stopped");
    break;
  case SYSTEM_EVENT_STA_CONNECTED:
    debugln("Connected to access point");
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
  {
    debugln("Disconnected from WiFi access point");
    // mqttclient.disconnect();
    break;
  }
  case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
    debugln("Authentication mode of access point has changed");
    break;
  case SYSTEM_EVENT_STA_GOT_IP:
  {
    debug("Obtained IP address: ");
    debugln(WiFi.localIP());
    break;
  }
  case SYSTEM_EVENT_STA_LOST_IP:
    debugln("Lost IP address and IP address is reset to 0");
    //      vTaskDelay( 5000 );
    //      ESP.restart();
    break;
  case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
    debugln("WiFi Protected Setup (WPS): succeeded in enrollee mode");
    break;
  case SYSTEM_EVENT_STA_WPS_ER_FAILED:
    debugln("WiFi Protected Setup (WPS): failed in enrollee mode");
    //      ESP.restart();
    break;
  case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
    debugln("WiFi Protected Setup (WPS): timeout in enrollee mode");
    break;
  case SYSTEM_EVENT_STA_WPS_ER_PIN:
    debugln("WiFi Protected Setup (WPS): pin code in enrollee mode");
    break;
  case SYSTEM_EVENT_AP_START:
    debugln("WiFi access point started");
    break;
  case SYSTEM_EVENT_AP_STOP:
    debugln("WiFi access point  stopped");
    //      WiFi.mode( WIFI_OFF);
    //      esp_sleep_enable_timer_wakeup( 1000000 * 2 ); // 1 second times how many seconds wanted
    //      esp_deep_sleep_start();
    break;
  case SYSTEM_EVENT_AP_STACONNECTED:
    debugln("Client connected");
    break;
  case SYSTEM_EVENT_AP_STADISCONNECTED:
    debugln("WiFi client disconnected");
    break;
  case SYSTEM_EVENT_AP_STAIPASSIGNED:
    debugln("Assigned IP address to client");
    break;
  case SYSTEM_EVENT_AP_PROBEREQRECVED:
    debugln("Received probe request");
    break;
  case SYSTEM_EVENT_GOT_IP6:
    debugln("IPv6 is preferred");
    break;
  case SYSTEM_EVENT_ETH_GOT_IP:
    debugln("Obtained IP address");
    break;
  default:
    break;
  }
}

// void init_wifi()
// {
//   Serial.print("Connect to Wifi");

//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);
//   WiFi.setAutoReconnect(true);

//   while (WiFi.status() != WL_CONNECTED)
//   {
//     vTaskDelay(250);
//     debug(".");
//   }

//   debugln("");
//   debugln("WiFi connected");
//   debugln("IP address: ");
//   debugln(WiFi.localIP());
//   debug("RSSI: ");
//   debugln(WiFi.RSSI());
//   debugln(Tools::get_wifi_quality(WiFi.RSSI()));
// }

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

  debug("MQTT connected");
  mqttclient.onMessage(onMessageReceived);
  mqttclient.subscribe(MQTT_INBOUND MY_ID "/#");
}

void init_mqtt()
{
  // note: https://forum.arduino.cc/t/once-wificlient-fails-it-never-connects-again/1045456/6
  // and this: https://forum.arduino.cc/t/once-wificlient-fails-it-never-connects-again/1045456/2
  if (WiFi.status() != WL_CONNECTED)
    return;

  mqttclient.begin(MQTT_BROKER, wifiClient);

  while (!mqttclient.connect(MY_ID "_mqtt"))
  {
    debug(".");
    delay(1000);
  }
  debug("MQTT connected");
  mqttclient.subscribe(MQTT_INBOUND MY_ID "/#");
  debug("MQTT connected to " MQTT_INBOUND MY_ID "/#");
  mqttclient.onMessage(onMessageReceived);
  mqttclient.publish("/hello", "world", false, 2);
}

// mqtt_client.setServer(MQTT_BROKER, 1883);
// mqtt_client.setBufferSize(2048);
// reconnect_mqtt();

// mqtt_client.subscribe(MQTT_INBOUND MY_ID "/#");
// mqtt_client.setCallback(proc_commands);

void check_wifi()
{
  // unsigned long currentMillis = millis();
  //  if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  // if (WiFi.status() != WL_CONNECTED)
  // {
  //   debug("WIFI is not connected anymore. Trying to reconnect")
  //       mqtt_client.disconnect();
  //   WiFi.disconnect();
  //   WiFi.reconnect();
  // }
}

bool every_25ms(void *argument)
{
  // pinHandler.loop();
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

bool every_30s(void *argument)
{
  check_wifi();
  reconnect_mqtt();
  return true;
}

void proc_commands(char *topic, byte *payload, unsigned int length)
{
  debug("Message arrived in topic: " + String(topic) + " with Payload: " + String((char *)payload));

  // last part of topic is the command
  char *cmd = strrchr(topic, '/');

  StaticJsonDocument<1024> incoming;
  DeserializationError jsonError = deserializeJson(incoming, (char *)payload);

  // Test if parsing succeeds.
  if (jsonError)
  {
    debug("deserializeJson() failed: ");
    debug(jsonError.f_str());
    return;
  }

  if (String(cmd) == String("/visual") || String(cmd) == String("/acoustic"))
  {
    // pinHandler.parse_incoming(cmd, incoming);
  }
  else if (String(cmd) == String("/timers"))
  {
    // int value = incoming.
  }
  incoming.clear();
}

void reconnect_mqtt()
{
  // makes no sense, as long as we don't have a network
  if (WiFi.status() != WL_CONNECTED)
    return;

  // while (!mqtt_client.connected())
  // {
  //   debug("searching mqtt");
  //   if (!mqtt_client.connect(MY_ID))
  //   {
  //     delay(5000);
  //   }
  //   else
  //   {
  //     debug("mqtt connected");
  //   }
  // }
}

void handleButton(Button2 &b)
{
  debug(b.getID());
  debug(b.isPressed() ? "DOWN" : "UP");
  StaticJsonDocument<32> button_event;
  button_event["button"] = b.isPressed() ? "down" : "up";
  // Topic: `/rlg/evt/ag01/btn01` or `/rlg/evt/ag01/btn02`
  button_event.clear();
}

void button_handler(Button2 &b)
{
  const String device = "/btn01";
  const char *topic = MQTT_OUTBOUND MY_ID "/btn0";

  StaticJsonDocument<32> button_event;
  button_event["button"] = b.isPressed() ? "down" : "up";
  String output;
  serializeJson(button_event, output);
  const char *payload = output.c_str();
  const char *t = topic;
  String t1 = String(topic) + String(b.getID());

  // mqtt_client.publish(t1.c_str(), payload, true);
  //  Topic: `/rlg/evt/ag01/btn01` or `/rlg/evt/ag01/btn02`
  button_event.clear();
}

void setup()
{
  Serial.begin(115200);

  connectToWiFi();
  mqttclient.begin(MQTT_BROKER, wifiClient);
  connectToMQTT();

  // btn01.begin(BTN01);
  // btn01.setID(1);
  // btn01.setPressedHandler(button_handler);
  // btn01.setReleasedHandler(button_handler);

  // btn02.begin(BTN02);
  // btn02.setID(2);
  // btn02.setPressedHandler(button_handler);
  // btn02.setReleasedHandler(button_handler);

  // timer.every(25, every_25ms);
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
  // btn01.loop();
  // btn02.loop();
  // btn02.loop();
}