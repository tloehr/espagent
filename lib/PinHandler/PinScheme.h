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

class PinScheme
{
private:
    Adafruit_MCP23X17 mcp;
    std::list<int> scheme;
    std::list<int> backup;
    long repeat;
    int pin_state;
    int pin;

public:
    PinScheme();
    PinScheme(Adafruit_MCP23X17 &mcp, int pin);
    void set_repeat(int r);
    void off();
    void clear_scheme();
    void add_scheme_entry(int entry);
    void backup_scheme();
    bool is_empty();
    void next();
};

#endif