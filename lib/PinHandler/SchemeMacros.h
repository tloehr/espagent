#ifndef SCHEME_MACROS_H
#define SCHEME_MACROS_H

#include <Arduino.h>

#include <ArduinoJson.h>

class SchemeMacros
{
private:
    StaticJsonDocument<1500> json;

public:
    SchemeMacros();
    JsonObject get_macro(String key, JsonObject default_scheme);
};

#endif