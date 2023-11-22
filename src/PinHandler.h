//
// Created by Torsten Löhr on 18.10.23.
//

#ifndef PINHANDLER_H
#define PINHANDLER_H

#include <iostream>
#include <list>
#include <string>
#include <ArduinoJson.h>
#include "defines.h"
#include <Adafruit_MCP23X17.h>

#define LED_WHITE "wht"
#define LED_RED "red"
#define LED_BLUE "blu"
#define LED_YELLOW "ylw"
#define LED_GREEN "grn"
#define SIREN1 "sir1"
#define SIREN2 "sir2"
#define SIREN3 "sir3"
#define SIREN4 "sir4"
#define BUZZER "buzzer"

#define PIN_WHITE GPA0
#define PIN_RED GPA1
#define PIN_BLUE GPA2
#define PIN_YELLOW GPA3
#define PIN_GREEN GPA4
#define PIN_SIREN1 GPA5
#define PIN_SIREN2 GPA6
#define PIN_SIREN3 GPA7
#define PIN_SIREN4 GPB0
#define PIN_BUZZER GPB1

#define PERIOD 25
#define NUMBER_OF_ACOUSTIC_PINS 5
#define NUMBER_OF_LED_PINS 5
#define NUMBER_OF_ALL_PINS 10
#define EOL "\n"

class PinHandler
{
private:
    Adafruit_MCP23X17 mcp;
    std::list<int> schemes[NUMBER_OF_ALL_PINS];
    std::list<int> backup_schemes[NUMBER_OF_ALL_PINS];
    long repeat[NUMBER_OF_ALL_PINS] = {};
    int pin_states[NUMBER_OF_ALL_PINS] = {};
    StaticJsonDocument<1500> scheme_macros;

    String all_keys[NUMBER_OF_ALL_PINS] = {LED_WHITE, LED_RED, LED_YELLOW, LED_GREEN, LED_BLUE, SIREN1, SIREN2, SIREN3, SIREN4, BUZZER};
    int gpio_assignment[NUMBER_OF_ALL_PINS] = {PIN_WHITE, PIN_RED, PIN_YELLOW, PIN_GREEN, PIN_BLUE, PIN_SIREN1, PIN_SIREN2, PIN_SIREN3, PIN_SIREN4, PIN_BUZZER};
    String led_all[5] = {LED_WHITE, LED_RED, LED_YELLOW, LED_GREEN, LED_BLUE};
    String sir_all[5] = {SIREN1, SIREN2, SIREN3, SIREN4, BUZZER};

    int find_in_keys(const String &element);
    void off(const String &);
    int gpio[NUMBER_OF_ALL_PINS] = {};

public:
    PinHandler();

    // void init();
    void parse_incoming(const String &cmd, StaticJsonDocument<1024> incoming);
    void loop();
};

#endif // UNTITLED2_PINHANDLER_H
