#include <DS3231.h>
#include <Adafruit_SSD1306.h>

class Display
{
public:
    Display();
    void initialize();
    void displayMoisture(int percentage);
    void displayTime(DateTime time);

private:
    Adafruit_SSD1306 display;
};