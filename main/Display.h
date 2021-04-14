#include <DS3231.h>
#include <Adafruit_SSD1306.h>

class Display
{
public:
    Display();
    void initialize(int rotation);
    void setRotation(int rotation);
    void displayMoisture(int percentage);
    void displayTime(DateTime time);
    void displayLastWatering(uint32_t now, uint32_t wateringTime);
    void displayPressTime(double percentage);

private:
    Adafruit_SSD1306 display;
};