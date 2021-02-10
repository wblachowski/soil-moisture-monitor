#include "Display.h"
#include <DS3231.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

Display::Display()
{
    display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
}

void Display::initialize()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        for (;;)
            ;
    }
    display.clearDisplay();
    display.setTextColor(WHITE, BLACK);
}

void Display::displayMoisture(int percentage)
{
    display.setTextSize(4);
    display.setCursor(0, 0);
    display.print("   ");
    display.setCursor(0, 0);
    display.print(percentage);
    display.print("%");
    display.display();
}

void Display::displayTime(DateTime time)
{
    display.setTextSize(1);

    display.setCursor(80, 0);
    if (time.hour() < 10)
    {
        display.print('0');
    }
    display.print(time.hour());
    display.print(':');
    if (time.minute() < 10)
    {
        display.print('0');
    }
    display.print(time.minute());
    display.print(':');
    if (time.second() < 10)
    {
        display.print('0');
    }
    display.print(time.second());

    display.setCursor(80, 15);
    if (time.day() < 10)
    {
        display.print('0');
    }
    display.print(time.day());
    display.print('.');
    if (time.month() < 10)
    {
        display.print('0');
    }
    display.print(time.month());
    display.print('.');
    display.print(time.year() % 100);
    display.display();
}