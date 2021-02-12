class ButtonHandler
{
public:
    ButtonHandler();
    unsigned long pressTime(int state, unsigned long now);
    void disableUntilChange();

private:
    int lastState;
    unsigned long lastChange;
    bool disabled;
};