#include "ButtonHandler.h"

ButtonHandler::ButtonHandler()
{
    lastState = 0;
    lastChange = 0L;
}

unsigned long ButtonHandler::pressTime(int state, unsigned long now)
{
    if (lastState != state)
    {
        lastState = state;
        lastChange = now;
    }
    if (state)
    {
        return now - lastChange;
    }
    return 0L;
}