class TimeGuard
{
public:
    TimeGuard(unsigned long interval);
    bool execute(unsigned long currentTime);

private:
    unsigned long interval;
    unsigned long lastExecution;
};