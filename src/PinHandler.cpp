//
// Created by Torsten Löhr on 18.10.23.
//

#include "PinHandler.h"

PinHandler::PinHandler()
{
    if (!mcp.begin_I2C())
    {
        Serial.println("Error.");
        while (1)
            ;
    }

    DeserializationError jsonError = deserializeJson(scheme_macros, "");

    for (int i = 0; i < NUMBER_OF_ALL_PINS; i++)
    {
        gpio[i] = gpio_assignment[i];
        mcp.pinMode(gpio[i], OUTPUT);
    }
}

/**
 * converts the scheme in the incoming json - stored under key - into an array of integers and puts this array into the list at the
 * right index for this key. it also chops the value into chunks of size PERIOD.
 *
 * Any pre existing array in that list will be overridden.
 *
 * @param json_str
 * @param key
 */
void PinHandler::parse_incoming(const String &cmd, StaticJsonDocument<1024> incoming)
{
    debug("parse_incoming");
    debug(cmd);

    // preprocess the json for the sir_all or led_all pin selection
    if (incoming.containsKey("sir_all"))
    {
        if (incoming["sir_all"] == "off")
            for (const String &key : sir_all)
                off(key);
        else
            // expand json for all siren pins
            for (const String &key : sir_all)
                incoming[key] = incoming["sir_all"];

        // now remove the sir_all definition from the json
        incoming.remove("sir_all");
    }
    if (incoming.containsKey("led_all"))
    {
        if (incoming["led_all"] == "off")
            for (const String &key : led_all)
                off(key);
        else
            // expand json for all siren pins
            for (const String &key : led_all)
                incoming[key] = incoming["led_all"];

        // now remove the sir_all definition from the json
        incoming.remove("led_all");
    }

    debug("received JSON ");
#ifdef DEBUG_MODE
    serializeJsonPretty(incoming, Serial);
#endif

    for (const String &key : all_keys)
    {
        if (!incoming.containsKey(key))
            continue;

        if (incoming[key] == "off")
        {
            off(key);
            continue;
        }

        if (!incoming[key].containsKey("repeat") || !incoming[key].containsKey("scheme"))
            continue;

        debug("found valid scheme for " + key);

        int indexOfKey = find_in_keys(key);

        pin_states[indexOfKey] = 0;

        // store the repeat value for this key
        repeat[indexOfKey] = incoming[key]["repeat"] < 0 ? LONG_MAX : incoming[key]["repeat"].as<int>() - 1;

        schemes[indexOfKey].clear();
        for (int iEl = 0; iEl < incoming[key]["scheme"].size(); iEl++)
        {
            int value = incoming[key]["scheme"].as<JsonArray>()[iEl];
            int multiplier = std::abs(value / PERIOD);
            int sign = value >= 0 ? 1 : -1;

            for (int i = 0; i < multiplier; i++)
                schemes[indexOfKey].push_back(PERIOD * sign);

            // backup the scheme for REPEAT handling
            backup_schemes[indexOfKey].clear();
            std::copy(schemes[indexOfKey].begin(), schemes[indexOfKey].end(),
                      std::back_inserter(backup_schemes[indexOfKey]));
        }
    }
}

void PinHandler::loop()
{
    for (const String &key : all_keys)
    {
        int indexOfKey = find_in_keys(key);
        if (indexOfKey >= NUMBER_OF_ALL_PINS)
            continue;
        if (schemes[indexOfKey].empty())
            continue;

        int scheme = schemes[indexOfKey].front();
        schemes[indexOfKey].pop_front();

        // init pin_state when entering the first time.
        if (pin_states[indexOfKey] == 0)
            pin_states[indexOfKey] = scheme;

        // check if pin is ON already
        if (pin_states[indexOfKey] != scheme)
        {
            mcp.digitalWrite(gpio[indexOfKey], scheme > 0 ? HIGH : LOW);
            pin_states[indexOfKey] = scheme;
        }

        // refill if necessary
        if (schemes[indexOfKey].empty() &&
            repeat[indexOfKey] > 0)
        { // last pop emptied the list, but we need to repeat at least once more
            debug("list is empty - refilling for repeat# " + String(repeat[indexOfKey]));

            // REPEAT
            // copy the backup to the scheme
            std::copy(backup_schemes[indexOfKey].begin(), backup_schemes[indexOfKey].end(),
                      std::back_inserter(schemes[indexOfKey]));

            repeat[indexOfKey]--;
        }
    }
}

int PinHandler::find_in_keys(const String &element)
{
    int found = NUMBER_OF_ALL_PINS;
    for (int i = 0; i < NUMBER_OF_ALL_PINS; ++i)
    {
        if (all_keys[i] == element)
        {
            found = i;
            break;
        }
    }
    return found;
}

void PinHandler::off(const String &key)
{
    debug("setting " + key + " to off");
    int indexOfKey = find_in_keys(key);
    schemes[indexOfKey].clear();
    backup_schemes[indexOfKey].clear();
    mcp.digitalWrite(gpio[indexOfKey], LOW);
}