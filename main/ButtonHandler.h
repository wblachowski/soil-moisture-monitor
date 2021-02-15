class ButtonHandler
{
public:
    ButtonHandler(int dbncTime);
    unsigned long pressTime(int state, unsigned long now);
    void disableUntilChange();

private:
    int lastState;
    unsigned long lastChange;
    int debounceTime;
    bool disabled;
};