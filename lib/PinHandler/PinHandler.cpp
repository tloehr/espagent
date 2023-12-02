//
// Created by Torsten Löhr on 18.10.23.
//

#include "PinHandler.h"

PinHandler::PinHandler()
{
}

void PinHandler::begin()
{
    // Wire.begin(I2C_SDA, I2C_SCL);
    while (!mcp.begin_I2C())
    {
        debugln("Error finding mcp23017 at 0x20.");
        vTaskDelay(1000);
    }

    // DeserializationError jsonError = deserializeJson(scheme_macros, "");

    for (int i = 0; i < NUMBER_OF_ALL_PINS; i++)
    {
        debugln("PinHandler " + String(i));
        PinScheme pinScheme(mcp, gpio_assignment[i]);
        pins[i] = pinScheme;
    }
}

/**
 * Parses the incoming json and decodes the scheme into the
 * appropriate PinScheme object. These objects are stored in the
 * pins array.
 *
 * Pin on/off duration values are "chopped" into multiple chunks with the size
 * of PERIOD (which is currently 25ms)
 *
 * Existing pinschemes will be overwritten immediately.
 *
 * @param incoming the json document containing the schemes from the rlgcommander.
 */
void PinHandler::parse_incoming(StaticJsonDocument<2048> incoming)
{
    // preprocess the json for the sir_all or led_all pin selection
    if (incoming.containsKey("sir_all"))
    {
        if (incoming["sir_all"] == "off")
            for (const String &key : sir_all)
                pins[find_in_keys(key)].off();
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
                pins[find_in_keys(key)].off();
        else
            // expand json for all siren pins
            for (const String &key : led_all)
                incoming[key] = incoming["led_all"];

        // now remove the sir_all definition from the json
        incoming.remove("led_all");
    }

#ifdef DEBUG_MODE
    debugln("received JSON ");
    serializeJsonPretty(incoming, Serial);
#endif

    for (const String &key : all_json_keys)
    {
        if (!incoming.containsKey(key))
            continue;

        if (incoming[key] == "off")
        {
            pins[find_in_keys(key)].off();
            continue;
        }

        if (!incoming[key].containsKey("repeat") || !incoming[key].containsKey("scheme"))
            continue;

        debugln("found valid scheme for " + key);

        int indexOfKey = find_in_keys(key);

        // store the repeat value for this key
        pins[indexOfKey].set_repeat(incoming[key]["repeat"] < 0 ? LONG_MAX : incoming[key]["repeat"].as<int>() - 1);
        pins[indexOfKey].clear_scheme();

        for (int iEl = 0; iEl < incoming[key]["scheme"].size(); iEl++)
        {
            // transforms a time like "100" to "25,25,25,25"
            // where PERIOD is 25
            int value = incoming[key]["scheme"].as<JsonArray>()[iEl];
            int multiplier = std::abs(value / PERIOD);
            int sign = value >= 0 ? 1 : -1;

            for (int i = 0; i < multiplier; i++)
                pins[indexOfKey].add_scheme_entry(PERIOD * sign);

            pins[indexOfKey].backup_scheme();
        }
    }
}

void PinHandler::loop()
{
    for (const String &key : all_json_keys)
    {
        int indexOfKey = find_in_keys(key);
        if (indexOfKey >= NUMBER_OF_ALL_PINS)
            continue;
        if (pins[indexOfKey].is_empty())
            continue;

        pins[indexOfKey].next();
    }
}

int PinHandler::find_in_keys(const String &element)
{
    int found = NUMBER_OF_ALL_PINS;
    for (int i = 0; i < NUMBER_OF_ALL_PINS; ++i)
    {
        if (all_json_keys[i] == element)
        {
            found = i;
            break;
        }
    }
    return found;
}