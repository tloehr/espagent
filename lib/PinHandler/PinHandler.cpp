//
// Created by Torsten Löhr on 18.10.23.
//

#include "PinHandler.h"

PinHandler::PinHandler()
{
}

void PinHandler::begin()
{
    while (!mcp.begin_I2C())
    {
        debugln("Error finding mcp23017 at 0x20.");
        vTaskDelay(1000);
    }
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
void PinHandler::parse_incoming(StaticJsonDocument<2048> &incoming)
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

        // check if macros were used
        // and replace if necessary
        // if (incoming[key].size() < 25 && scheme_macros.has(incoming[key]))
        //{
        //    incoming[key] = scheme_macros.get_macro(incoming[key]);
        //}

        // some structure check - rudimentary
        if (!incoming[key].containsKey("repeat") || !incoming[key].containsKey("scheme"))
            continue;

        debugln("found valid scheme for " + key);

        int indexOfKey = find_in_keys(key);

        int repeat = incoming[key]["repeat"] < 0 ? INT_MAX : incoming[key]["repeat"].as<int>() - 1;
        JsonArray scheme = incoming[key]["scheme"].as<JsonArray>();
        pins[indexOfKey].init(repeat, scheme);
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