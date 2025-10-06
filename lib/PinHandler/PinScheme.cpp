#include "PinScheme.h"

PinScheme::PinScheme()
{
}

PinScheme::PinScheme(Adafruit_MCP23X17 &mcp, int pin)
{
    this->mcp = mcp;
    this->pin = pin;
    pin_state = 0;
    scheme_length = 0;
    pointer = 0;
    repeat = 0;
    mcp.pinMode(pin, OUTPUT);
}

void PinScheme::set_repeat(int repeat)
{
    this->repeat = repeat;
}

void PinScheme::clear()
{
    pointer = 0;
    repeat = 0;
    scheme_length = 0;
    delete scheme;
    mcp.digitalWrite(pin, LOW);
}

void PinScheme::init(int repeat, JsonArray &json_scheme)
{
    this->pointer = 0;
    this->repeat = repeat;
    this->scheme_length = json_scheme.size();
    this->scheme = new int[scheme_length];

    for (int n = 0; n < scheme_length; n++)
    {
        int value = json_scheme[n];
        int multiplier = std::abs(value / PERIOD);
        int sign = value >= 0 ? 1 : -1;

        for (int i = 0; i < multiplier; i++)
            scheme[i] = PERIOD * sign;
    }
}

/**
 * sets the scheme pointer to the next value and switches the pin to the new state (on and off)
 * if the end of the scheme is reached and there are still REPEATs > 0, the scheme
 * will be restored from the backup_scheme.
 *
 */
void PinScheme::next()
{
    if (scheme_length == 0)
        return;
    int new_state = scheme[pointer];
    if (pointer == 0)
        pin_state = new_state;
    pointer++;

    // check if pin is ON already
    if (pin_state != new_state)
    {
        mcp.digitalWrite(pin, new_state > 0 ? HIGH : LOW);
        pin_state = new_state;
    }

    // refill if necessary
    if (pointer >= scheme_length)
    {
        if (repeat > 0)
        { // last next() reached the end of the list, but we need to repeat at least once more
            debugln("list is empty - repeating # " + String(repeat));
            pointer = 0;
            repeat--;
        }
        else // no more repeats ?
            clear();
    }
}