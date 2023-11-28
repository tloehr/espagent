#include "PinScheme.h"

PinScheme::PinScheme()
{
}

PinScheme::PinScheme(Adafruit_MCP23X17 &mcp, int pin)
{
    this->mcp = mcp;
    this->pin = pin;
    pin_state = 0;
    mcp.pinMode(pin, OUTPUT);
}

void PinScheme::set_repeat(int repeat)
{
    this->repeat = repeat;
}

void PinScheme::off()
{
    debug("setting  to off");
    scheme.clear();
    backup.clear();
    mcp.digitalWrite(pin, LOW);
}

void PinScheme::clear_scheme()
{
    scheme.clear();
}

void PinScheme::add_scheme_entry(int entry)
{
    scheme.push_back(entry);
}

void PinScheme::backup_scheme()
{
    backup.clear();
    std::copy(scheme.begin(), scheme.end(),
              std::back_inserter(backup));
}

bool PinScheme::is_empty()
{
    return scheme.empty();
}

/**
 * sets the scheme pointer to the next value and switches the pin to the new state (on and off)
 * if the end of the scheme is reached and there are still REPEATs to be done, the scheme
 * will be restored from the backup_scheme.
 *
 */
void PinScheme::next()
{
    int front = scheme.front();
    if (pin_state == 0)
        pin_state = front;
    scheme.pop_front();

    // check if pin is ON already
    if (pin_state != front)
    {
        mcp.digitalWrite(pin, front > 0 ? HIGH : LOW);
        pin_state = front;
    }

    // refill if necessary
    if (scheme.empty())
    {
        if (repeat > 0)
        { // last pop emptied the list, but we need to repeat at least once more
            debugln("list is empty - refilling for repeat# " + String(repeat));

            // REPEAT
            // copy the backup to the scheme
            std::copy(backup.begin(), backup.end(),
                      std::back_inserter(scheme));

            repeat--;
        }
        else // no more repeats ? then we clear the backup, too
        {
            backup.clear();
        }
    }
}