#include "Tools.h"

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

void Tools::log_wifi_event(WiFiEvent_t event)
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
