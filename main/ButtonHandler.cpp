#include "ButtonHandler.h"

ButtonHandler::ButtonHandler()
{
    lastState = 0;
    lastChange = 0L;
    disabled = false;
}

unsigned long ButtonHandler::pressTime(int state, unsigned long now)
{

    if (lastState != state)
    {
        lastState = state;
        lastChange = now;
        disabled = false;
    }

    if (disabled)
    {
        return 0L;
    }

    if (state)
    {
        return now - lastChange;
    }
    return 0L;
}

void ButtonHandler::disableUntilChange()
{
    disabled = true;
}