class ButtonHandler
{
public:
    ButtonHandler();
    unsigned long pressTime(int state, unsigned long now);

private:
    int lastState;
    unsigned long lastChange;
};