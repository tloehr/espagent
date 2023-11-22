#include "Tools.h"
#include "defines.h"

/**
 * for the esp32 the rssi value is the same as the dbm
 */
String Tools::get_wifi_quality(int dbm)
{
    String wifiQuality = "";

    if (dbm >= 0)
        wifiQuality = "ERROR";
    else if (dbm >= WIFI_PERFECT)
        wifiQuality = "PERFECT";
    else if (dbm > WIFI_GOOD)
        wifiQuality = "good";
    else if (dbm > WIFI_FAIR)
        wifiQuality = "fair";
    else if (dbm > WIFI_MINIMUM)
        wifiQuality = "fair";
    else if (dbm > WIFI_UNSTABLE)
        wifiQuality = "bad"; // bad
    else if (dbm > WIFI_BAD)
        wifiQuality = "bad"; // bad
    else
        wifiQuality = "no_wifi"; // no wifi

    return wifiQuality;
}
