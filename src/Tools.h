#ifndef TOOLS_H
#define TOOLS_H

#include "WString.h"
#include <iostream>
#include "defines.h"
#include <WiFi.h>

class Tools
{
private:
    static const int WIFI_PERFECT = -30;
    static const int WIFI_EXCELLENT = -50;
    static const int WIFI_GOOD = -60;
    static const int WIFI_FAIR = -67;
    static const int WIFI_MINIMUM = -70;
    static const int WIFI_UNSTABLE = -80;
    static const int WIFI_BAD = -90;
    static const int NO_WIFI = -99;

public:
    static String get_wifi_quality(int dbm);
    static void log_wifi_event(WiFiEvent_t event);
};

#endif