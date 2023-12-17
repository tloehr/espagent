//
// Created by Torsten Löhr on 27.11.23
// Keeps information for schemes everything for exactly ONE pin
//
#ifndef PINSCHEME_H
#define PINSCHEME_H

#include <iostream>
#include <list>
#include <string>
#include "../../include/defines.h"
#include <Adafruit_MCP23X17.h>
#include <ArduinoJson.h>

#define PERIOD 25

class PinScheme
{
private:
    Adafruit_MCP23X17 mcp;
    int scheme[2048];
    int scheme_length;
    int repeat;
    int pin_state;
    int pin;
    int pointer;

public:
    PinScheme();
    PinScheme(Adafruit_MCP23X17 &mcp, int pin);
    void set_repeat(int r);
    void init(int repeat, JsonArray &json_scheme);
    void clear();
    void add_scheme_entry(int entry);
    void backup_scheme();
    bool is_empty();
    void next();
};

#endif