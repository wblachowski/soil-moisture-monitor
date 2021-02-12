#include "TimeGuard.h"

TimeGuard::TimeGuard(unsigned long intrvl)
{
    interval = intrvl;
    lastExecution = 0L;
}

bool TimeGuard::execute(unsigned long currentTime)
{
    if (lastExecution == 0L || currentTime - lastExecution > interval)
    {
        lastExecution = currentTime;
        return true;
    }
    return false;
}