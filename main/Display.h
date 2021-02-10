#include <DS3231.h>
#include <Adafruit_SSD1306.h>

class Display
{
public:
    Display();
    void initialize();
    void displayMoisture(int percentage);
    void displayTime(DateTime time);
    void displayLastWatering(uint32_t now, uint32_t wateringTime);

private:
    Adafruit_SSD1306 display;
};